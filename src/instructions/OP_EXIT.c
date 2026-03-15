#include "OP_EXIT.h"

void op_exit_search_operands(RegistersBank *bank, Bus *bus, Memory *mem) {
    // nada
}

void op_exit_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags) {
    *flags |= STOP;
}

void op_exit_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags) {
    // nada
}