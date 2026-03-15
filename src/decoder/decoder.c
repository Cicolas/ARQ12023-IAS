#include "./decoder.h"

void mem_to_file(FILE *file, Memory *memoria) {
    for (int i = 0; i < memoria->size; i++) {
        const WORD word = mem_read(memoria, i, MASK);

        fprintf(file, "%lu\n", word);
    }
}