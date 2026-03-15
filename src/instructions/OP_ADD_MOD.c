#include "OP_ADD_MOD.h"

void op_add_mod_search_operands(RegistersBank *bank, Bus *bus, Memory *mem) {
    search_operands(bank, bus, mem);
}

void op_add_mod_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags) {
    // if (iteracao == 0) ULA_execute(ula, rMBR_read(bank), rAC_read(bank), SOMAR);
    if (iteracao == 0) ALU_execute(ula, rMBR_read(bank), rAC_read(bank), MOD_SUM);
}

void op_add_mod_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags) {
    rAC_load(bank, ula->saida);
}