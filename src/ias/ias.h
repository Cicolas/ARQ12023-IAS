#ifndef IAS_H_
#define IAS_H_

#include <stdint.h>
#include "../memory/memory.h"
#include "../bus/bus.h"
#include "../cpu/cpu.h"
#include "../utils.h"

typedef struct {
    bool running;
} IASConfig;

typedef struct {
    IASConfig config;
    CPU *cpu;
    Bus *barramento;
    Memory *memoria;
} IAS;

IAS *IAS_create(CPU *cpu, Bus *barramento, Memory *memoria);
void IAS_free(IAS *ias);
void IAS_start(IAS *ias, WORD PC);
void IAS_tick(IAS *ias);

#endif