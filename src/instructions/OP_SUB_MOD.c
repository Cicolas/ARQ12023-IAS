#include "OP_SUB_MOD.h"

void op_sub_mod_search_operands(RegistersBank *bank, Bus *bus, Memory *mem) {
    search_operands(bank, bus, mem);
}

void op_sub_mod_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags) {
    if (iteracao == 0) ALU_execute(ula, rAC_read(bank), rMBR_read(bank), MOD_SUBTRACT);
}

void op_sub_mod_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags) {
    rAC_load(bank, ula->saida);
}