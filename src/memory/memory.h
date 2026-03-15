#ifndef MEMORIA_H_
#define MEMORIA_H_

#include <stdint.h>
#include "../defs/defs.h"
#include "../bus/bus.h"

typedef struct {
    int size;
    int data_size;
    // 4096 * 40
    // 4096 * 5 * 8
    uint8_t *data;
} Memory;

Memory *mem_create(int size, int data_size);
void memoria_free(Memory *memoria);
void mem_write(const Memory *memoria, uint addr, WORD word, WORD mask);
WORD mem_read(const Memory *memoria, uint addr, WORD mask);
void mem_add_instruction(const Memory *memoria, INSTRUCTION op, ARGUMENT arg, uint addr);
void mem_tick(const Memory *memoria, Bus *barramento);

#endif