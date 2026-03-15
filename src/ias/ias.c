#include "./ias.h"

IAS *IAS_create(CPU *cpu, Bus *barramento, Memory *memoria) {
    IAS *ias = malloc(sizeof(IAS));

    ias->config = (IASConfig){
        .running = false,
    };
    ias->cpu = cpu;
    ias->barramento = barramento;
    ias->memoria = memoria;

    return ias;
}

void IAS_free(IAS *ias) {
    CPU_free(ias->cpu);
    bus_free(ias->barramento);
    memoria_free(ias->memoria);
    free(ias);
}

void IAS_start(IAS *ias, const WORD PC) {
    ias->config.running = true;
    ias->cpu->registers_bank.rPC = PC;
}

void IAS_tick(IAS *ias) {
    CPU_tick(ias->cpu, ias->barramento, ias->memoria);

    ias->config.running = (char)!(ias->cpu->cu->pipeline->flags & STOP) ;
}