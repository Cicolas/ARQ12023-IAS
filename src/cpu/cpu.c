#include "./cpu.h"

CPU *CPU_create(void) {
    CPU *cpu = malloc(sizeof(CPU));

    cpu->registers_bank = (RegistersBank){
        .rAC  = 0,
        .rMQ  = 0,
        .rMBR = 0,
        .rPC  = 0,
        .rMAR = 0,
        .rIBR = 0,
        .rIR  = 0,
    };
    cpu->cu = UC_create();
    cpu->alu = ALU_create();

    return cpu;
}

void CPU_free(CPU *cpu) {
    UC_free(cpu->cu);
    ALU_free(cpu->alu);
    free(cpu);
}

void CPU_tick(CPU *cpu, Bus *barramento, Memory *memoria) {
    UC_tick(cpu->cu, cpu->alu, &cpu->registers_bank, barramento, memoria);
}