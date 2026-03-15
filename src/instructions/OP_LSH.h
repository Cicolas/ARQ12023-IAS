#ifndef OP_LSH_H_
#define OP_LSH_H_

#include "../utils.h"
#include "../defs/defs.h"
#include "../registers/registers.h"
#include "../uc/uc.h"
#include "../ula/ula.h"
#include "../memory/memory.h"
#include "../bus/bus.h"
#include "./defaults/search_operands.h"
#include "../pipeline/pipeline.h"

#define ADD_OP_LSH(pipeline) CONFIG_INSTRUCAO(pipeline, op_lsh, OP_LSH)

void op_lsh_search_operands(RegistersBank *bank, Bus *bus, Memory *mem);
void op_lsh_execute(int iteracao, RegistersBank *bank, ALU *ula, PipelineFlag *flags);
void op_lsh_write_results(RegistersBank *bank, Bus *bus, Memory *mem, ALU *ula, PipelineFlag *flags);

#endif