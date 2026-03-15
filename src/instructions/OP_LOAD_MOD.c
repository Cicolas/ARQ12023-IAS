#include "OP_LOAD_MOD.h"

void op_load_mod_search_operands(RegistersBank *bank, Bus *bus, Memory *mem) {
    search_operands(bank, bus, mem);
}

void op_load_mod_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags) {
    if (iteracao == 0) ALU_execute(ula, rMBR_read(bank), 0, MOD);
}

void op_load_mod_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags) {
    rAC_load(bank, ula->saida);
}