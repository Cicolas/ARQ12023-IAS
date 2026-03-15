#ifndef REGISTRADORES_H_
#define REGISTRADORES_H_

#include <stdint.h>
#include "../defs/defs.h"

typedef struct {
    WORD rAC;
    WORD rMQ;
    WORD rMBR;
    WORD rPC;
    WORD rMAR;
    WORD rIBR;
    WORD rIR;
} RegistersBank;

void rPC_increment(RegistersBank *banco);
void rIBR_reset(RegistersBank *banco);

void rAC_load(RegistersBank *banco, WORD origin);
void rMQ_load(RegistersBank *banco, WORD origin);
void rMBR_load(RegistersBank *banco, WORD origin);
void rPC_load(RegistersBank *banco, WORD origin);
void rMAR_load(RegistersBank *banco, WORD origin, WORD mask);
void rIBR_load(RegistersBank *banco, WORD origin);
void rIR_load(RegistersBank *banco, WORD origin, WORD mask);

WORD rMQ_read(const RegistersBank *banco);
WORD rMAR_read(const RegistersBank *banco);
WORD rPC_read(const RegistersBank *banco);
WORD rMBR_read(const RegistersBank *banco);
WORD rAC_read(const RegistersBank *banco);

#endif