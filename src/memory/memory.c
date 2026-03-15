#include "./memory.h"

Memory *mem_create(const int size, const int data_size) {
    Memory *memoria = malloc(sizeof(Memory));

    memoria->size = size;
    memoria->data_size = data_size;
    memoria->data = calloc(size * 5, sizeof(memoria->data));

    return memoria;
}

void memoria_free(Memory *memoria) {
    free(memoria->data);
    free(memoria);
}

void mem_write(const Memory *memoria, const uint addr, WORD word, const WORD mask) {
    if (addr >= SIZE) {
        RAISE("Invalid address: '%d'", addr);
    }

    const WORD old_word = mem_read(memoria, addr, MASK) & ~mask;
    word &= mask;
    word |= old_word;

    PRINT("%i %ld %ld", addr, old_word, word);
    const uint anchor = addr * 5;

    memoria->data[anchor]     = (word & 0xFF00000000) >> 32; // 0xFF = 0b11111111
    memoria->data[anchor + 1] = (word & 0x00FF000000) >> 24;
    memoria->data[anchor + 2] = (word & 0x0000FF0000) >> 16;
    memoria->data[anchor + 3] = (word & 0x000000FF00) >>  8;
    memoria->data[anchor + 4] =  word & 0x00000000FF       ;
}

WORD mem_read(const Memory *memoria, const uint addr, const WORD mask) {
    if (addr >= SIZE) {
        RAISE("Invalid address: '%d'", addr);
    }

    const uint anchor = addr * 5;

    const WORD word =
        (WORD)memoria->data[anchor]     << 32 |
        (WORD)memoria->data[anchor + 1] << 24 |
        (WORD)memoria->data[anchor + 2] << 16 |
        (WORD)memoria->data[anchor + 3] << 8  |
        (WORD)memoria->data[anchor + 4]       ;
    
    return word & mask;
}

void mem_add_instruction(const Memory *memoria, const INSTRUCTION op, const ARGUMENT arg, const uint addr) {
    WORD word = mem_read(memoria, addr, MASK);
    const WORD operation_arg = (WORD)op << 12 | (WORD)arg;
    
    if ((word & RIGHT_MASK) != 0)
        word <<= 20;

    word |= operation_arg;

    mem_write(memoria, addr, word, MASK);
}

void mem_tick(const Memory *memoria, Bus *barramento) {
    WORD mask = MASK;
    if (barramento->control == PARTIAL_STORE_L)
        mask = ARGUMENT_MASK << 20;
    if (barramento->control == PARTIAL_STORE_R)
        mask = ARGUMENT_MASK;

    switch (barramento->control) {
        case NONE: {
            WARN("mem_tick() called with Control Bus in \"NONE\" state");
            break;
        }
        case LOAD: {
            const WORD word = mem_read(memoria, addr_bus(barramento), mask);
            data_bus_write(barramento, word);
            break;
        }
        case PARTIAL_STORE_L:
        case PARTIAL_STORE_R:
        case STORE: {
            const WORD word = data_bus_read(barramento);
            mem_write(memoria, addr_bus(barramento), word, mask);
            break;
        }
        default:
            RAISE("Control Bus has a unsupported value! (%i)", barramento->control);
    }
}