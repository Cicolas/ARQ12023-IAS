#ifndef ESCREVER_RESULTADOS_MEMORIA_H_
#define ESCREVER_RESULTADOS_MEMORIA_H_

#include <stdint.h>
#include "../../registers/registers.h"
#include "../../bus/bus.h"
#include "../../memory/memory.h"
#include "../../defs/defs.h"

void write_results_memory(RegistersBank *bank, Bus *bus, Memory *mem, WORD mask);

#endif
