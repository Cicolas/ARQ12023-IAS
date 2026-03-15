#ifndef OP_STOR_L_H_
#define OP_STOR_L_H_

#include "../utils.h"
#include "../defs/defs.h"
#include "../registers/registers.h"
#include "../uc/uc.h"
#include "../ula/ula.h"
#include "../memory/memory.h"
#include "../bus/bus.h"
#include "./defaults/search_operands.h"
#include "./defaults/write_results_memory.h"
#include "../pipeline/pipeline.h"

#define ADD_OP_STOR_L(pipeline) CONFIG_INSTRUCAO(pipeline, op_stor_l, OP_STOR_L)

void op_stor_l_search_operands(RegistersBank *bank, Bus *bus, Memory *mem);
void op_stor_l_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags);
void op_stor_l_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags);

#endif