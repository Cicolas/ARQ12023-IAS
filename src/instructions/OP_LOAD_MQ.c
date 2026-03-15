#include "OP_LOAD_MQ.h"

void op_load_mq_search_operands(RegistersBank *bank, Bus *bus, Memory *mem) {
    // nada
}

void op_load_mq_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags) {
    // nada
}

void op_load_mq_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags) {
    rAC_load(bank, rMQ_read(bank));
}