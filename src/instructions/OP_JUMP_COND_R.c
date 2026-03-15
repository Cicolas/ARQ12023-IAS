#include "OP_JUMP_COND_R.h"

void op_jump_cond_r_search_operands(RegistersBank *bank, Bus *bus, Memory *mem) {
    search_operands(bank, bus, mem);
    rMBR_load(bank, rMBR_read(bank) & RIGHT_MASK);
}

void op_jump_cond_r_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags) {
    if (iteracao == 0) ALU_execute(ula, rAC_read(bank), 0, GREATER_THAN);
}

void op_jump_cond_r_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags) {
    if (ula->saida) {
        rPC_load(bank, rMAR_read(bank));
        rIBR_load(bank, rMBR_read(bank));
        *flags |= PIPELINE_FLUSH;
    }
}