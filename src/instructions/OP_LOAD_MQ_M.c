#include "OP_LOAD_MQ_M.h"

void op_load_mq_m_search_operands(RegistersBank *bank, Bus *bus, Memory *mem) {
    search_operands(bank, bus, mem);
}

void op_load_mq_m_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags) {
    if (iteracao == 0) ALU_execute(ula, rMBR_read(bank), 0, MOVE);
}

void op_load_mq_m_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags) {
    rMQ_load(bank, ula->saida);
}