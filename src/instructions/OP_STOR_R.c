#include "OP_STOR_R.h"

void op_stor_r_search_operands(RegistersBank *bank, Bus *bus, Memory *mem) {
    // funcao de busca de operandos
}

void op_stor_r_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags) {
    if (iteracao == 0) ALU_execute(ula, rAC_read(bank), 0, MOVE);
}

void op_stor_r_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags) {
    // TODO: ESCRITA PARCIAL NA MEMORIA
    rMBR_load(bank, ula->saida);
    write_results_memory(bank, bus, mem, RIGHT_MASK);
    *flags |= STOR_PARTIALLY_FINISHED;
}