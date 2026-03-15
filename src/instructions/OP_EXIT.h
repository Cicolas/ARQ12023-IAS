#ifndef OP_EXIT_H_
#define OP_EXIT_H_

#include "../utils.h"
#include "../defs/defs.h"
#include "../registers/registers.h"
#include "../uc/uc.h"
#include "../ula/ula.h"
#include "../memory/memory.h"
#include "../bus/bus.h"

#define ADD_OP_EXIT(pipeline) CONFIG_INSTRUCAO(pipeline, op_exit, OP_EXIT)

void op_exit_search_operands(RegistersBank *bank, Bus *bus, Memory *mem);
void op_exit_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags);
void op_exit_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags);

#endif
