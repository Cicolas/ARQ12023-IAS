#ifndef OP_LOAD_MQ_M_H_
#define OP_LOAD_MQ_M_H_

#include "../utils.h"
#include "../defs/defs.h"
#include "../registers/registers.h"
#include "../uc/uc.h"
#include "../ula/ula.h"
#include "../memory/memory.h"
#include "../bus/bus.h"
#include "./defaults/search_operands.h"
#include "../pipeline/pipeline.h"

#define ADD_OP_LOAD_MQ_M(pipeline) CONFIG_INSTRUCAO(pipeline, op_load_mq_m, OP_LOAD_MQ_M)

void op_load_mq_m_search_operands(RegistersBank *bank, Bus *bus, Memory *mem);
void op_load_mq_m_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags);
void op_load_mq_m_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags);

#endif