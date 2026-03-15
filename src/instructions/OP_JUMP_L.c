#include "OP_JUMP_L.h"

void op_jump_l_search_operands(RegistersBank *bank, Bus *bus, Memory *mem) {
    // search_operands(bank, bus, mem);
}

void op_jump_l_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags) {
    // funcao de execucao
}

void op_jump_l_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags) {
    rPC_load(bank, rMAR_read(bank));
    rIBR_reset(bank);
    *flags |= PIPELINE_FLUSH;
}