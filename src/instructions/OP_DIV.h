#ifndef OP_DIV_H_
#define OP_DIV_H_

#include "../utils.h"
#include "../defs/defs.h"
#include "../registers/registers.h"
#include "../uc/uc.h"
#include "../ula/ula.h"
#include "../memory/memory.h"
#include "../bus/bus.h"
#include "./defaults/search_operands.h"

#define ADD_OP_DIV(pipeline) CONFIG_INSTRUCAO(pipeline, op_div, OP_DIV)

void op_div_search_operands(RegistersBank *bank, Bus *bus, Memory *mem);
void op_div_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags);
void op_div_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags);

#endif