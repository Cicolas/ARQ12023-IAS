#ifndef OP_LOAD_MQ_H_
#define OP_LOAD_MQ_H_

#include "../utils.h"
#include "../defs/defs.h"
#include "../registers/registers.h"
#include "../uc/uc.h"
#include "../ula/ula.h"
#include "../memory/memory.h"
#include "../bus/bus.h"
#include "./defaults/search_operands.h"

#define ADD_OP_LOAD_MQ(pipeline) CONFIG_INSTRUCAO(pipeline, op_load_mq, OP_LOAD_MQ)

void op_load_mq_search_operands(RegistersBank *bank, Bus *bus, Memory *mem);
void op_load_mq_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags);
void op_load_mq_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags);

#endif
