#ifndef BARRAMENTO_H_
#define BARRAMENTO_H_

#include <stdint.h>
#include <stdlib.h>
#include "../defs/defs.h"
#include "../registers/registers.h"

typedef enum {
    NONE,
    LOAD,
    STORE,
    PARTIAL_STORE_L,
    PARTIAL_STORE_R
} CONTROL_BUS_FUNCTION;

typedef struct {
    CONTROL_BUS_FUNCTION control;
    WORD data;
    WORD address;
} Bus;

Bus *bus_create(void);
void bus_free(Bus *bus);

void addr_bus_write(Bus *bus, WORD word);
WORD addr_bus(const Bus *bus);
void data_bus_write(Bus *bus, WORD word);
WORD data_bus_read(const Bus *bus);
void control_bus_set(Bus *bus, CONTROL_BUS_FUNCTION func);

#endif