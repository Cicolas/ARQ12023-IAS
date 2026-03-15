#include "OP_STOR.h"

void op_stor_search_operands(RegistersBank *bank, Bus *bus, Memory *mem) {
    // funcao de busca de operandos
}

void op_stor_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags) {
    if (iteracao == 0) {
        ALU_execute(ula, rAC_read(bank), 0, MOVE);
        *flags |= STOR_EXECUTED;
    }
}

void op_stor_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags) {
    rMBR_load(bank, ula->saida);
    write_results_memory(bank, bus, mem, MASK);
}