#include "OP_MUL.h"

void op_mul_search_operands(RegistersBank *bank, Bus *bus, Memory *mem) {
    search_operands(bank, bus, mem);
}

void op_mul_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags) {
    if (iteracao == 0) ALU_execute(ula, rMBR_read(bank), rMQ_read(bank), MULTIPLY);
}

void op_mul_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags) {
    rMQ_load(bank, ula->saida);
    rAC_load(bank, ula->overflow);
}