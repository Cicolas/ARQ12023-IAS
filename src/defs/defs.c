#include "defs.h"

char *optoa(INSTRUCTION op) {
    switch (op)
    {
        case (INSTRUCTION)0b00010100:
            return "OP_LSH";
        case (INSTRUCTION)0b00010101:
            return "OP_RSH";
        case (INSTRUCTION)0b00000001:
            return "OP_LOAD";
        case (INSTRUCTION)0b00100001:
            return "OP_STOR";
        case (INSTRUCTION)0b00000010:
            return "OP_LOAD_MENOS";
        case (INSTRUCTION)0b00000011:
            return "OP_LOAD_MOD";
        case (INSTRUCTION)0b00000100:
            return "OP_LOAD_MENOS_MOD";
        case (INSTRUCTION)0b00001010:
            return "OP_LOAD_MQ";
        case (INSTRUCTION)0b00001001:
            return "OP_LOAD_MQ_M";
        case (INSTRUCTION)0b00001101:
            return "OP_JUMP_L";
        case (INSTRUCTION)0b00001110:
            return "OP_JUMP_R";
        case (INSTRUCTION)0b00001111:
            return "OP_JUMP_COND_L";
        case (INSTRUCTION)0b00010000:
            return "OP_JUMP_COND_R";
        case (INSTRUCTION)0b00000101:
            return "OP_ADD";
        case (INSTRUCTION)0b00000111:
            return "OP_ADD_MOD";
        case (INSTRUCTION)0b00000110:
            return "OP_SUB";
        case (INSTRUCTION)0b00001000:
            return "OP_SUB_MOD";
        case (INSTRUCTION)0b00001011:
            return "OP_MUL";
        case (INSTRUCTION)0b00001100:
            return "OP_DIV";
        case (INSTRUCTION)0b00010010:
            return "OP_STOR_L";
        case (INSTRUCTION)0b00010011:
            return "OP_STOR_R";
        case (INSTRUCTION)0b11111111:
            return "OP_EXIT";       
        default:
            return "unknown";
    }
}

WORD i64toi40(int64_t number) {
    const bool negative = (bool)(number < 0);
    number = llabs(number);
    number &= MASK >> 1;
    number |= negative ? 1ll << 39: 0;

    return number;
}

int64_t i40toi64(const WORD word) {
    const bool negative = (bool)(word >> 39);
    const int64_t num = (int64_t)(word & MASK >> 1);

    return negative ? -num : num;
}
