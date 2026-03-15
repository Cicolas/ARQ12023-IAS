#include "./ula.h"

ALU *ALU_create(void) {
    ALU *ula = malloc(sizeof(ALU));

    ula->saida = 0;
    ula->overflow = 0;

    return ula;
}

void ALU_free(ALU *ula) {
    free(ula);
}

void ALU_execute(ALU *ula, const WORD a, const WORD b, const ALUOperationEnum alu_op) {
    const int64_t a_i64 = i40toi64(a);
    const int64_t b_i64 = i40toi64(b);

    switch (alu_op) {
        case MOVE:
            ula->saida = a;
            break;
        case SUM:
            ula->saida = i64toi40(a_i64 + b_i64);
            break;
        case SUBTRACT:
            ula->saida = i64toi40(a_i64 - b_i64);
            PRINT("SUB %ld - %ld = %ld", a_i64, b_i64, ula->saida);
            break;
        case MOD_SUBTRACT:
            ula->saida = i64toi40(a_i64 - b_i64);
            ula->saida &= MASK >> 1;
            break;
        case MULTIPLY: {
            // TODO: saida de 80 bits
            const WORD mul = i64toi40(a_i64 * b_i64);
            ula->saida = mul & MASK;
            ula->overflow = mul & ~MASK;
            break;
        }
        case DIVIDE:
            ula->saida = i64toi40(a_i64 / b_i64);
            ula->overflow = i64toi40(a_i64 % b_i64);
            break;
        case MOD_SUM:
            ula->saida = i64toi40(a_i64 + b_i64);
            ula->saida &= MASK >> 1;
            break;
        case GREATER_THAN:
            ula->saida = (bool)(a_i64 >= b_i64);
            PRINT("GREATER THAN %ld >= %ld = %ld", a_i64, b_i64, ula->saida);
            break;
        case NEGATIVE:
            ula->saida = a | 1ll << 39;
            break;
        case MOD:
            ula->saida = a & MASK >> 1;
            break;
        case LSH:
            ula->saida = a << 1;
            break;
        case RSH:
            ula->saida = a >> 1;
            break;
        default:
            RAISE("Unknow ALU Opeartion! (%i)", alu_op);
    }
}
