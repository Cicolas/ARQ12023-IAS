#ifndef TRADUTOR_H_
#define TRADUTOR_H_

#include "../defs/defs.h"
#include "../utils.h"
#include "../memory/memory.h"
#include "../ias/ias.h"
#include "../decoder/decoder.h"

void compile_to_file(char *in, char *out, int size, int data_size);
void compilar_para_memoria(FILE *in, Memory *memoria);
void compile_to_IAS(FILE *in, const IAS *ias);

#endif