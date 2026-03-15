#ifndef DECODER_H_
#define DECODER_H_

#include <stdio.h>  
#include "../utils.h"
#include "../memory/memory.h"

void mem_to_file(FILE *file, Memory *memoria);
void file_to_mem(FILE *file, Memory *memoria);

#endif