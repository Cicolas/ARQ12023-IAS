#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "utils.h"
#include "defs/defs.h"
#include "parser/parser.h"
#include "instructions/instructions.h"

#define CLOCK_SPEED 1000 // Hz
#define CLOCKLESS false

double timeofday_ms(const struct timeval tv) {
    return ((double)tv.tv_usec / 1000000.) + ((double)tv.tv_sec);
}

void clock_update(IAS *ias, const int speed) {
    const double tick_time = 1./speed;
    struct timeval tv_start,
                   tv_elapsed;

    gettimeofday(&tv_start, NULL);
    double time_start = timeofday_ms(tv_start);

    while (ias->config.running) {
        gettimeofday(&tv_elapsed, NULL);
        const double time_elapsed = timeofday_ms(tv_elapsed);

        if (CLOCKLESS || (time_elapsed - time_start) > tick_time) {
            IAS_tick(ias);
            time_start = time_elapsed;
        }
    }
}

void configure_instructions(const CPU *cpu) {
    add_instructions(cpu->cu->pipeline);
}

int main(const int argc, char **argv) {
    if (argc <= 4) {
        RAISE("Arguments number incompatible");
    }

    if (strcmp(argv[1], "-p") == 0) {
        char *in = argv[2];

        FILE *f = fopen(in, "r");
        if (!f) {
            RAISE("File '%s' not found", in);
        }

        if (strcmp(argv[3], "-i") != 0) {
            RAISE("Unknown command: '%s'", argv[3]);
        }
        const int data_length = atoi(argv[4]);

        struct timeval time_start,
                       time_end;
        gettimeofday(&time_start, NULL);

        Memory *memoria = mem_create(SIZE, data_length);
        CPU *cpu = CPU_create();
        Bus *barramento = bus_create();

        configure_instructions(cpu);
        IAS *ias = IAS_create(cpu, barramento, memoria);

        compile_to_IAS(f, ias);
        PRINT("Compiled sucessfully!");

        IAS_start(ias, data_length);
        clock_update(ias, CLOCK_SPEED);

        gettimeofday(&time_end, NULL);
        PRINT("Program finished (%lf s) sucessfully!", timeofday_ms(time_end) - timeofday_ms(time_start));

        const char *filename = "build/out.ias.d";
        FILE *f_out = fopen(filename, "w");
        if (!f_out) {
            RAISE("File '%s' not found", "build/out.ias.d");
        }

        mem_to_file(f_out, memoria);
        IAS_free(ias);
    } else if (strcmp(argv[1], "-t") == 0) {
        char *in = argv[2];
        
        if (strcmp(argv[3], "-m") != 0) {
            RAISE("Unknown command: '%s'", argv[3]);
        }
        char *out = argv[4];

        if (strcmp(argv[5], "-i") != 0) {
            RAISE("Unknown command: '%s'", argv[5]);
        }
        int data_length = atoi(argv[6]);

        compile_to_file(in, out, SIZE, data_length);
    } else {
        RAISE("Unknown command: '%s'", argv[1]);
    }
    
    return 0;
}