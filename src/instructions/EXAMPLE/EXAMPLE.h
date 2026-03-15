#if 0
#ifndef <OP_NAME>_H_
#define <OP_NAME>_H_

#include "../utils.h"
#include "../defs/defs.h"
#include "../bus/bus.h"
#include "../uc/uc.h"
#include "../alu/alu.h"
#include "../memory/memory.h"
#include "../bus/bus.h"
#include "./defaults/search_operands.h"

#define ADD_<OP_NAME>(pipeline) CONFIG_INSTRUCAO(pipeline, <op_name>, <OP_NAME>)

void <op_name>_search_operands(BancoRegistradores *bank, Barramento *bus, Memoria *mem);
void <op_name>_execute(int iteracao, BancoRegistradores *bank, ULA *ula);
void <op_name>_write_results(BancoRegistradores *bank, Barramento *bus, Memoria *mem, ULA *ula);

#endif
#endif