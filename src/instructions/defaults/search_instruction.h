#ifndef BUSCAR_INSTRUCAO_H_
#define BUSCAR_INSTRUCAO_H_

#include "../../registers/registers.h"
#include "../../bus/bus.h"
#include "../../memory/memory.h"

void search_instruction(RegistersBank *bank, Bus *bus, Memory *mem);

#endif
