#ifndef DEFS_H_
#define DEFS_H_

#include "../utils.h"
#include <stdint.h>
#include <stdlib.h>

#define SIZE 4096
// #define TAMANHO_REAL TAMANHO * 5
// int TAMANHO_DADOS = 500;

// TODO: WRITE A MORE ACCURATE IMPLEMENTATION OF 40 bits WORD SIZE
#define WORD uint64_t
#define INSTRUCTION uint8_t
#define ARGUMENT uint16_t
#define MASK (uint64_t)0xFFFFFFFFFF // 0b1111111111111111111111111111111111111111
#define OP_MASK (uint64_t)0xFF000
#define ARGUMENT_MASK (uint64_t)0xFFF
#define LEFT_MASK (uint64_t)0xFFFFF00000
#define RIGHT_MASK (uint64_t)0x00000FFFFF

#define NUM_INSTRUCOES      22
#define OP_LSH              (INSTRUCTION)0b00010100
#define OP_RSH              (INSTRUCTION)0b00010101
#define OP_LOAD             (INSTRUCTION)0b00000001
#define OP_STOR             (INSTRUCTION)0b00100001
#define OP_LOAD_MENOS       (INSTRUCTION)0b00000010
#define OP_LOAD_MOD         (INSTRUCTION)0b00000011
#define OP_LOAD_MENOS_MOD   (INSTRUCTION)0b00000100
#define OP_LOAD_MQ          (INSTRUCTION)0b00001010
#define OP_LOAD_MQ_M        (INSTRUCTION)0b00001001

#define OP_JUMP_L           (INSTRUCTION)0b00001101
#define OP_JUMP_R           (INSTRUCTION)0b00001110

#define OP_JUMP_COND_L      (INSTRUCTION)0b00001111
#define OP_JUMP_COND_R      (INSTRUCTION)0b00010000

#define OP_ADD              (INSTRUCTION)0b00000101
#define OP_ADD_MOD          (INSTRUCTION)0b00000111
#define OP_SUB              (INSTRUCTION)0b00000110
#define OP_SUB_MOD          (INSTRUCTION)0b00001000
#define OP_MUL              (INSTRUCTION)0b00001011
#define OP_DIV              (INSTRUCTION)0b00001100

#define OP_STOR_L           (INSTRUCTION)0b00010010
#define OP_STOR_R           (INSTRUCTION)0b00010011

#define OP_EXIT             (INSTRUCTION)0b11111111

typedef char PipelineFlag;

#define PIPELINE_FLUSH          (PipelineFlag)0b0000001
#define STOP                    (PipelineFlag)0b0000010
#define STOR_EXECUTED           (PipelineFlag)0b0000100
#define STOR_PARTIALLY_FINISHED (PipelineFlag)0b0001000
#define LOAD_RIGHT              (PipelineFlag)0b0010000

char *optoa(INSTRUCTION op);
WORD i64toi40(int64_t number);
int64_t i40toi64(WORD word);

#endif