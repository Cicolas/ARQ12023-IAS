#ifndef OP_JUMP_L_H_
#define OP_JUMP_L_H_

#include "../utils.h"
#include "../defs/defs.h"
#include "../registers/registers.h"
#include "../uc/uc.h"
#include "../ula/ula.h"
#include "../memory/memory.h"
#include "../bus/bus.h"
#include "./defaults/search_operands.h"
#include "../pipeline/pipeline.h"

#define ADD_OP_JUMP_L(pipeline) CONFIG_INSTRUCAO(pipeline, op_jump_l, OP_JUMP_L)

void op_jump_l_search_operands(RegistersBank *bank, Bus *bus, Memory *mem);
void op_jump_l_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags);
void op_jump_l_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags);

#endif