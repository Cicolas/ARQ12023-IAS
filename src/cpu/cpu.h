#ifndef CPU_H_
#define CPU_H_

#include <stdio.h>
#include <stdlib.h>
#include "../utils.h"
#include "../uc/uc.h"
#include "../ula/ula.h"
#include "../bus/bus.h"
#include "../registers/registers.h"

typedef struct {
    RegistersBank registers_bank;
    UC *cu;
    ALU *alu;
} CPU;

CPU *CPU_create(void);
void CPU_free(CPU *cpu);
void CPU_tick(CPU *cpu, Bus *barramento, Memory *memoria);

#endif