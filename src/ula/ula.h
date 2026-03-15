#ifndef ULA_H_
#define ULA_H_

#include <stdint.h>
#include "../defs/defs.h"

typedef enum {
    MOVE,
    SUM,
    MOD_SUM,
    SUBTRACT,
    MOD_SUBTRACT,
    MULTIPLY,
    DIVIDE,
    GREATER_THAN,
    NEGATIVE,
    MOD,
    LSH,
    RSH
} ALUOperationEnum;

typedef struct {
    WORD saida;
    WORD overflow;
} ALU;

ALU *ALU_create(void);
void ALU_free(ALU *ula);
void ALU_execute(ALU *ula, WORD a, WORD b, ALUOperationEnum alu_op);

#endif