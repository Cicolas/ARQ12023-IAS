#include "OP_RSH.h"

void op_rsh_search_operands(RegistersBank *bank, Bus *bus, Memory *mem) {
    // funcao de busca de operandos
}

void op_rsh_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags) {
    if (iteracao == 0) ALU_execute(ula, rAC_read(bank), 0, RSH);
}

void op_rsh_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags) {
    rAC_load(bank, ula->saida);
}