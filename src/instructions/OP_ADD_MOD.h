#ifndef OP_ADD_MOD_H_
#define OP_ADD_MOD_H_

#include "../utils.h"
#include "../defs/defs.h"
#include "../registers/registers.h"
#include "../uc/uc.h"
#include "../ula/ula.h"
#include "../memory/memory.h"
#include "../bus/bus.h"
#include "./defaults/search_operands.h"

#define ADD_OP_ADD_MOD(pipeline) CONFIG_INSTRUCAO(pipeline, op_add_mod, OP_ADD_MOD)

void op_add_mod_search_operands(RegistersBank *bank, Bus *bus, Memory *mem);
void op_add_mod_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags);
void op_add_mod_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags);

#endif