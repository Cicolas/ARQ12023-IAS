#include "OP_LOAD_SUB.h"

void op_load_menos_search_operands(RegistersBank *bank, Bus *bus, Memory *mem) {
    search_operands(bank, bus, mem);
}

void op_load_menos_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags) {
    if (iteracao == 0) ALU_execute(ula, rMBR_read(bank), 0, NEGATIVE);
}

void op_load_menos_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags) {
    rAC_load(bank, ula->saida);
}