#include "./registers.h"

void rPC_increment(RegistersBank *banco) {
    banco->rPC++;
}

void rIBR_reset(RegistersBank *banco) {
    banco->rIBR = 0;
}

void rAC_load(RegistersBank *banco, const WORD origin) {
    banco->rAC = origin;
}

void rMQ_load(RegistersBank *banco, const WORD origin) {
    banco->rMQ = origin;
}

void rMBR_load(RegistersBank *banco, const WORD origin) {
    banco->rMBR = origin;
}

void rPC_load(RegistersBank *banco, const WORD origin) {
    banco->rPC = origin;
}

void rMAR_load(RegistersBank *banco, const WORD origin, const WORD mask) {
    const uint shift = mask == LEFT_MASK ? 20 : 0;
    const WORD mask_real = ARGUMENT_MASK << shift;

    banco->rMAR = (origin & mask_real) >> shift;
}

void rIBR_load(RegistersBank *banco, const WORD origin) {
    banco->rIBR = origin & RIGHT_MASK;
}

void rIR_load(RegistersBank *banco, const WORD origin, const WORD mask) {
    const uint shift = mask == LEFT_MASK ? 20 : 0;
    const WORD mask_real = OP_MASK << shift;

    banco->rIR = (origin & mask_real) >> (shift + 12);
}

WORD rMQ_read(const RegistersBank *banco) {
    return banco->rMQ;
}

WORD rMAR_read(const RegistersBank *banco) {
    return banco->rMAR;
}

WORD rPC_read(const RegistersBank *banco) {
    return banco->rPC;
}

WORD rMBR_read(const RegistersBank *banco) {
    return banco->rMBR;
}

WORD rAC_read(const RegistersBank *banco) {
    return banco->rAC;
}
