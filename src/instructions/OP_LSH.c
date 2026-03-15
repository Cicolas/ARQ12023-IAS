#include "OP_LSH.h"

void op_lsh_search_operands(RegistersBank *bank, Bus *bus, Memory *mem) {
    // funcao de busca de operandos
}

void op_lsh_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags) {
    if (iteracao == 0) ALU_execute(ula, rAC_read(bank), 0, LSH);
}

void op_lsh_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags) {
    rAC_load(bank, ula->saida);
}