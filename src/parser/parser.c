#include "./parser.h"

#define MOD_PIPE    (char)0b0001
#define MOD_MINUS   (char)0b0010
#define MOD_M       (char)0b0100
#define MOD_PLUS    (char)0b1000

typedef enum {
    OPERATION,
    NUMBER,

    MEMORY,
    COMMA,
    PIPE,
    PAREN_OPEN,
    PAREN_CLOSE,
    MINUS,
    COLON,
    EMPTY,
    MORE
} SymbolEnum;

typedef struct {
    SymbolEnum type;
    char value[128];
} Symbol;

typedef enum {
    MULTIVALUE,
    SINGLE
} MemoryFunctionEnum;

typedef struct {
    MemoryFunctionEnum type;
    int value;
    char addr;
} ParseMemResult;

static Symbol next_symbol(char **line);
static Symbol peek_symbol(char *line);
static bool is_number(const char *str);
static Symbol except_next_symbol(char **line, SymbolEnum type, int line_num);
static void compile_config_line(char *line, const CPU *cpu, int line_num) ;
static void compile_line(char *line, const Memory *mem, int line_num);
static void compile_config_section(FILE *in, CPU *cpu);
static void compile_data_section(FILE *in, const Memory *memoria);
static void compile_program_section(FILE *in, Memory *memoria);
static long parse_number(char **line, int line_num);
static char parse_range(char **line, int line_num);
static ParseMemResult parse_memory(char **line, int line_num);
static ParseMemResult except_memory(char **line, int line_num, MemoryFunctionEnum type);
static ParseMemResult except_memory_MOD(char **line, int line_num);
static void process_modifier(char **linha, char *modifiers, SymbolEnum symbol, char mod_type);

static char charhigh(char c);
static char fpeek(FILE *f);
static char *simtoa(SymbolEnum symbol);

//-Funcoes-------------------------------------------------------------------------------------------------------------------

void compile_to_file(char *in, char *out, const int size, const int data_size) {
    FILE *f = fopen(in, "r");
    if (!f) {
        RAISE("File '%s' not found", in);
    }
    FILE *f_out = fopen(out, "w");
    if (!f_out) {
        RAISE("File '%s' not found", out);
    }

    Memory *memoria = mem_create(size, data_size);

    PRINT("COMPILE TO MEMORY STARTED!");
    compilar_para_memoria(f, memoria);
    mem_to_file(f_out, memoria);

    memoria_free(memoria);
    fclose(f);
    fclose(f_out);
}

void compilar_para_memoria(FILE *in, Memory *memoria) {
    if (memoria == NULL) {
        RAISE("Memory cannot be NULL");
    }

    compile_config_section(in, NULL);
    compile_data_section(in, memoria);
    compile_program_section(in, memoria);
}

void compile_to_IAS(FILE *in, const IAS *ias) {
    if (ias == NULL) {
        RAISE("IAS cannot be NULL");
    }
    if (ias->memoria == NULL) {
        RAISE("Memoria cannot be NULL");
    }
    if (ias->cpu == NULL) {
        RAISE("cpu cannot be NULL");
    }

    compile_config_section(in, ias->cpu);
    compile_data_section(in, ias->memoria);
    compile_program_section(in, ias->memoria);
}

//-Funcoes estaticas---------------------------------------------------------------------------------------------------------

static void compile_config_section(FILE *in, CPU *cpu) {
    char line[128] = "\0";
    int n = 0;
    int op_len = 0;
    bool end = false;

    if (fpeek(in) == '/') {
        fgetc(in);
        fgetc(in);
        fgetc(in);
    }

    while (!end && n < NUM_INSTRUCOES) {
        const char c = charhigh((char) fgetc(in));

        if (c == '\n' || c == EOF) {
            end = (bool)(c == EOF);

            compile_config_line(line, cpu, n++);
            op_len = 0;

            line[0] = fpeek(in);
            if (line[0] == '*') {
                line[0] = (char)fgetc(in);
                line[1] = (char)fgetc(in);
                break;
            }
            line[2] = '\0';

            if (is_number(line)) break;
        } else if (c == ' ') {
            if (op_len > 0 && line[op_len - 1] != ' ') {
                line[op_len++] = ' ';
            }
        } else {
            line[op_len++] = c;
        }

        line[op_len] = '\0';
    }
}

static void compile_data_section(FILE *in, const Memory *memoria) {
    for (int i = 0; i < memoria->data_size; i++) {
        int64_t data = 0;
        if (fscanf(in, "%li", &data) == EOF) {
            RAISE("Error while reading data in this position: %d", i);
        }

        const WORD word = i64toi40(data);

        mem_write(memoria, (int16_t)i, word, MASK);

        if (fpeek(in) != '\n') break;
        fgetc(in);
    }
}

static void compile_program_section(FILE *in, Memory *memoria) {
    char line[128] = "\0";
    int op_len = 0;
    int i = memoria->data_size;
    bool end = false;

    while (!end) {
        const char c = charhigh((char) fgetc(in));
        
        if (c == '\n' || c == EOF) {
            end = (bool)(c == EOF);

            compile_line(line, memoria, i++);
            op_len = 0;
        } else if (c == ' ') {
            if (op_len > 0 && line[op_len - 1] != ' ') {
                line[op_len++] = ' ';
            }
        } else {
            line[op_len++] = c;
        }

        line[op_len] = '\0';
    }
}

static void compile_config_line(char *line, const CPU *cpu, const int line_num) {
    const Symbol symbol = except_next_symbol(&line, OPERATION, line_num);
    const char *operation_name = symbol.value;
    char modifiers = 0;
    INSTRUCTION operation = 0;

    process_modifier(&line, &modifiers, MORE, MOD_PLUS);
    process_modifier(&line, &modifiers, MINUS, MOD_MINUS);
    process_modifier(&line, &modifiers, PIPE, MOD_PIPE);
    process_modifier(&line, &modifiers, MEMORY, MOD_M);
    
    if (strcmp(operation_name, "LSH") == 0) {
        operation = OP_LSH;
    
    } else if (strcmp(operation_name, "RSH") == 0) {
        operation = OP_RSH;
    
    } else if (strcmp(operation_name, "LOADM") == 0) {
        operation = OP_LOAD_MQ;

    } else if (strcmp(operation_name, "LOADMM") == 0) {
        operation = OP_LOAD_MQ_M;
    
    } else if (strcmp(operation_name, "LOAD") == 0) {
        if (modifiers & MOD_M) {
            modifiers &= ~MOD_M;

            if (modifiers & MOD_MINUS && modifiers & MOD_PIPE) {
                modifiers &= ~MOD_MINUS;
                modifiers &= ~MOD_PIPE;
                
                operation = OP_LOAD_MENOS_MOD;
            } else if (modifiers & MOD_PIPE){
                modifiers &= ~MOD_PIPE;
              
                operation = OP_LOAD_MOD;
            } else if (modifiers & MOD_MINUS) {
                modifiers &= ~MOD_MINUS;
               
                operation = OP_LOAD_MENOS;
            } else {
                RAISE("invalid operation or invalid modifiers in '%s' (mod: %i)", operation_name, modifiers);
            }
        } else if (modifiers == 0) {
            operation = OP_LOAD;
        } else {
            RAISE("combinacao de modificadores (%i) inválida em 'LOAD'", modifiers);
        }
    } else if (strcmp(operation_name, "STOR") == 0) {
        operation = OP_STOR;
    
    }  else if (strcmp(operation_name, "STORM") == 0) {
        operation = OP_STOR_L;
    
    } else if (strcmp(operation_name, "JUMP") == 0) {
        if (modifiers & MOD_PLUS) {
            modifiers &= ~MOD_PLUS;

            operation = OP_JUMP_COND_L;
        } else if (modifiers == 0) {
            operation = OP_JUMP_L;
        }
    
    } else if (strcmp(operation_name, "ADD") == 0) {
        if (modifiers & MOD_PIPE) {
            modifiers &= ~MOD_PIPE;

            operation = OP_ADD_MOD;
        } else if (modifiers == 0) {
            operation = OP_ADD;
        }   
    } else if (strcmp(operation_name, "SUB") == 0) {
        if (modifiers & MOD_PIPE) {
            modifiers &= ~MOD_PIPE;

            operation = OP_SUB_MOD;
        } else if (modifiers == 0) {
            operation = OP_SUB;
        }
    } else if (strcmp(operation_name, "MUL") == 0) {
        operation = OP_MUL;
    } else if (strcmp(operation_name, "DIV") == 0) {
        operation = OP_DIV;
    } else if (strcmp(operation_name, "EXIT") == 0) {
        operation = OP_EXIT;
    }

    if (operation == 0) {
        RAISE("operacao inválida '%s' (mod: %i)", operation_name, modifiers);
    }
    if (modifiers != 0) {
        RAISE("invalid operation or invalid modifiers in '%s' (mod: %i)", operation_name, modifiers);
    }

    except_next_symbol(&line, COLON, line_num);
    const char num = (char) parse_number(&line, line_num);

    if(cpu == NULL) {
        WARN("CPU is NULL, this only should happen if the flag '-t' is provided");
        return;
    }

    PRINT("configuracao: '%s'", optoa(operation));

    pipeline_insert_op_time(cpu->cu->pipeline, operation, num);
}

static void compile_line(char *line, const Memory *mem, const int line_num) {
    const Symbol symbol = except_next_symbol(&line, OPERATION, line_num);
    const char *operation_name = symbol.value;
    const int addr = (line_num - mem->data_size) / 2 + mem->data_size;

    PRINT("%i: %s", line_num, operation_name);
    
    if (strcmp(operation_name, "LSH") == 0 || strcmp(operation_name, "RSH") == 0 || strcmp(operation_name, "EXIT") == 0) {
        if (operation_name[0] == 'L')
            mem_add_instruction(mem, OP_LSH, 0, addr);
        else if (operation_name[0] == 'R')
            mem_add_instruction(mem, OP_RSH, 0, addr);
        else
            mem_add_instruction(mem, OP_EXIT, 0, addr);
    }

    if (strcmp(operation_name, "LOAD") == 0) {
        const Symbol peeked_symbol = peek_symbol(line);

        if (peeked_symbol.type == OPERATION && strcmp(peeked_symbol.value, "MQ") == 0) {
            next_symbol(&line);
            const Symbol other_argument = peek_symbol(line);
            
            if (other_argument.type == COMMA) {
                next_symbol(&line);
                const ParseMemResult mem_op = except_memory(&line, line_num, SINGLE);

                mem_add_instruction(mem, OP_LOAD_MQ_M, mem_op.value, addr);
            } else if (other_argument.type == EMPTY) {
                mem_add_instruction(mem, OP_LOAD_MQ, 0, addr);
            } else {
                RAISE("Invalid argument: '%i' not allowed in 'load mq'", other_argument.type);
            }
        } else if (peeked_symbol.type == MINUS) {
            except_next_symbol(&line, MINUS, line_num);

            const Symbol other_argument = peek_symbol(line);
            
            if (other_argument.type == PIPE) {
                const ParseMemResult mem_mod = except_memory_MOD(&line, line_num);
                
                mem_add_instruction(mem, OP_LOAD_MENOS_MOD, mem_mod.value, addr);
            } else if (other_argument.type == MEMORY) {
                const ParseMemResult mem_op = except_memory(&line, line_num, SINGLE);
                
                mem_add_instruction(mem, OP_LOAD_MENOS, mem_op.value, addr);
            } else {
                RAISE("Invalid argument: '%i' not allowed in 'load -'", other_argument.type);
            }
        } else if (peeked_symbol.type == PIPE) {
            const ParseMemResult mem_mod = except_memory_MOD(&line, line_num);
            
            mem_add_instruction(mem, OP_LOAD_MOD, mem_mod.value, addr);
        } else if (peeked_symbol.type == MEMORY) {
            const ParseMemResult mem_op = except_memory(&line, line_num, SINGLE);
            
            mem_add_instruction(mem, OP_LOAD, mem_op.value, addr);
        }
    }

    if (strcmp(operation_name, "STOR") == 0) {
        const ParseMemResult mem_op = parse_memory(&line, line_num);
            
        if (mem_op.type == MULTIVALUE) {
            if (mem_op.addr == 'L') {
                mem_add_instruction(mem, OP_STOR_L, mem_op.value, addr);
            }
            else if (mem_op.addr == 'R') {
                mem_add_instruction(mem, OP_STOR_R, mem_op.value, addr);
            } else {
                RAISE("Invalid positioning for STOR");
            }
        } else {
            mem_add_instruction(mem, OP_STOR, mem_op.value, addr);
        }
    }

    if (strcmp(operation_name, "JUMP") == 0) {
        const Symbol peeked_symbol = peek_symbol(line);

        if (peeked_symbol.type == MORE) {
            next_symbol(&line);

            const ParseMemResult mem_op = except_memory(&line, line_num, MULTIVALUE);
            
            if (mem_op.addr == 'l')
                mem_add_instruction(mem, OP_JUMP_COND_L, mem_op.value, addr);
            else
                mem_add_instruction(mem, OP_JUMP_COND_R, mem_op.value, addr);
        } else {
            const ParseMemResult mem_op = except_memory(&line, line_num, MULTIVALUE);
            
            if (mem_op.addr == 'l')
                mem_add_instruction(mem, OP_JUMP_L, mem_op.value, addr);
            else
                mem_add_instruction(mem, OP_JUMP_R, mem_op.value, addr);
        }
    }

    if (strcmp(operation_name, "ADD") == 0) {
        const Symbol peeked_symbol = peek_symbol(line);

        if (peeked_symbol.type == MEMORY) {
            const ParseMemResult mem_op = except_memory(&line, line_num, SINGLE);

            mem_add_instruction(mem, OP_ADD, mem_op.value, addr);
        } else if (peeked_symbol.type == PIPE) {
            const ParseMemResult mem_op = except_memory_MOD(&line, line_num);

            mem_add_instruction(mem, OP_ADD_MOD, mem_op.value, addr);
        } else {
            RAISE("Invalid argument: '%i' not allowed in 'add'", peeked_symbol.type);
        }
    }

    if (strcmp(operation_name, "SUB") == 0) {
        const Symbol peeked_symbol = peek_symbol(line);

        if (peeked_symbol.type == MEMORY) {
            const ParseMemResult mem_op = except_memory(&line, line_num, SINGLE);

            mem_add_instruction(mem, OP_SUB, mem_op.value, addr);
        } else if (peeked_symbol.type == PIPE) {
            const ParseMemResult mem_op = except_memory_MOD(&line, line_num);

            mem_add_instruction(mem, OP_SUB_MOD, mem_op.value, addr);
        } else {
            RAISE("Invalid argument: '%i' not allowed in 'add'", peeked_symbol.type);
        }
    }

    if (strcmp(operation_name, "MUL") == 0) {
        const ParseMemResult mem_op = except_memory(&line, line_num, SINGLE);
            
        mem_add_instruction(mem, OP_MUL, mem_op.value, addr);
    }

    if (strcmp(operation_name, "DIV") == 0) {
        const ParseMemResult mem_op = except_memory(&line, line_num, SINGLE);
            
        mem_add_instruction(mem, OP_DIV, mem_op.value, addr);
    }

    except_next_symbol(&line, EMPTY, line_num);
}    

static Symbol next_symbol(char **line) {
    Symbol s = {0};
    int i = 0;

    for (; **line == ' '; (*line)++) {}

    for (; 
            **line != ' '  &&
            **line != '\n' &&
            **line != '\0' &&
            **line != ','  &&
            **line != '|'  &&
            **line != '('  &&
            **line != ')'  &&
            **line != '-'  &&
            **line != ':'  &&
            **line != '+'  ;
            (*line)++
        ) {
        s.value[i] = **line;
        s.value[i+1] = '\0';
        i++;
    }

    if (i == 0) {
        switch (**line) {
            case ',':
                s.type = COMMA;
                break;
            case '|':
                s.type = PIPE;
                break;
            case '(':
                s.type = PAREN_OPEN;
                break;
            case ')':
                s.type = PAREN_CLOSE;
                break;
            case '-':
                s.type = MINUS;
                break;
            case ':':
                s.type = COLON;
                break;
            case '+':
                s.type = MORE;
                break;
            default:
                s.type = EMPTY;
                break;
        }
        s.value[0] = **line;
        s.value[1] = '\0';
        (*line)++;
        return s;
    }

    if (strcmp(s.value, "M") == 0) {
        s.type = MEMORY;
        return s;
    }

    if (is_number(s.value)) s.type = NUMBER;
    else s.type = OPERATION;

    return s;
}

static Symbol peek_symbol(char *line) {
    return next_symbol(&line);
}

static bool is_number(const char *str) {
    size_t i;
    for (i = 0; (str[i] >= 48 && str[i] <= 57) || str[i] == '-'; i++) {}
    return (bool)(i == strlen(str));
}

static Symbol except_next_symbol(char **line, SymbolEnum type, int line_num) {
    const Symbol s = next_symbol(line);

    if (s.type != type) {
        RAISE("on the line '%i': A symbol should be '%s' but '%s' was found", line_num, simtoa(type), simtoa(s.type));
    }

    return s;
}

static long parse_number(char **line, const int line_num) {
    return atol(except_next_symbol(line, NUMBER, line_num).value);
}

static char parse_range(char **line, const int line_num) {
    const ARGUMENT begin_num = parse_number(line, line_num) & ARGUMENT_MASK;
    except_next_symbol(line, COLON, line_num);
    const ARGUMENT end_num = parse_number(line, line_num) & ARGUMENT_MASK;

    if (begin_num == 0 && end_num == 19) return 'l';
    if (begin_num == 8 && end_num == 19) return 'L';
    if (begin_num == 20 && end_num == 39) return 'r';
    if (begin_num == 28 && end_num == 39) return 'R';

    RAISE("'%i:%i' its not a valid range!", begin_num, end_num);
}

static ParseMemResult parse_memory(char **line, const int line_num) {
    except_next_symbol(line, MEMORY, line_num);
    except_next_symbol(line, PAREN_OPEN, line_num);

    const ARGUMENT arg = parse_number(line, line_num) & ARGUMENT_MASK;

    const Symbol proximo = next_symbol(line);
    if (proximo.type == COMMA) {
        const char range = parse_range(line, line_num);
        except_next_symbol(line, PAREN_CLOSE, line_num);

        return (ParseMemResult){
            .type = MULTIVALUE,
            .value = arg,
            .addr = range
        };
    }

    if (proximo.type == PAREN_CLOSE) {
        return (ParseMemResult){
            .type = SINGLE,
            .value = arg,
            .addr = 0
        };
    }

    RAISE("Invalid MEMORY function declaration");
}

static ParseMemResult except_memory(char **line, const int line_num, MemoryFunctionEnum type) {
    const ParseMemResult memoria = parse_memory(line, line_num);

    if (memoria.type != type) {
        RAISE("Wrong memory type");
    }

    return memoria;
}

static ParseMemResult except_memory_MOD(char **line, int line_num) {
    except_next_symbol(line, PIPE, line_num);
    const ParseMemResult memoria = except_memory(line, line_num, SINGLE);
    except_next_symbol(line, PIPE, line_num);

    return memoria;
}

static char charhigh(const char c) {
    return c >= 'a' && c <= 'z' ? c - 32 : c; // NOLINT(*-narrowing-conversions)
}

static char fpeek(FILE *f) {
    const char c = (char) fgetc(f);
    ungetc(c, f);

    return c;
}

static char *simtoa(const SymbolEnum symbol) {
    switch(symbol) {
        case OPERATION:
            return "OPERATION";
        case NUMBER:
            return "NUMBER";
        case MEMORY:
            return "MEMORY";
        case COMMA:
            return "COMMA";
        case PIPE:
            return "PIPE";
        case PAREN_OPEN:
            return "PAREN_OPEN";
        case PAREN_CLOSE:
            return "PAREN_CLOSE";
        case MINUS:
            return "MINUS";
        case COLON:
            return "COLON";
        case EMPTY:
            return "EMPTY";
        case MORE:
            return "MORE";
        default:
            return "Unknown";
    }
}

static void process_modifier(char **linha, char *modifiers, const SymbolEnum symbol, const char mod_type) {
    const Symbol peeked_symbol = peek_symbol(*linha);
    
    if (peeked_symbol.type == symbol) {
        next_symbol(linha);
        *modifiers |= mod_type; // NOLINT(*-narrowing-conversions)
    }
}