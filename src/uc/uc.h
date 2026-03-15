#ifndef UC_H_
#define UC_H_

#include <stdint.h>
#include "../defs/defs.h"
#include "../memory/memory.h"
#include "../bus/bus.h"
#include "../ula/ula.h"
#include "../utils.h"
#include "../pipeline/pipeline.h"
#include "../instructions/defaults/search_instruction.h"

typedef struct {
    Pipeline *pipeline;
} UC;

UC *UC_create(void);
void UC_free(UC *uc);

void UC_tick(const UC *uc, ALU *alu, RegistersBank *banco, Bus *barramento, Memory *memoria);

#endif