#include "OP_DIV.h"

void op_div_search_operands(RegistersBank *bank, Bus *bus, Memory *mem) {
    search_operands(bank, bus, mem);
}

void op_div_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags) {
    if (iteracao == 0) ALU_execute(ula, rAC_read(bank), rMBR_read(bank), DIVIDE);
}

void op_div_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags) {
    rMQ_load(bank, ula->saida);
    rAC_load(bank, ula->overflow);
}