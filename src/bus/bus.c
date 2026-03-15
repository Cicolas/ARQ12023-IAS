#include "./bus.h"

Bus *bus_create(void) {
    Bus *bus = malloc(sizeof(Bus));

    bus->control = NONE;
    bus->data = 0;
    bus->address = 0;

    return bus;
}

void bus_free(Bus *bus) {
    free(bus);
}

void addr_bus_write(Bus *bus, WORD pal) {
    bus->address = pal;
}

WORD addr_bus(const Bus *bus) {
    return bus->address;
}

void data_bus_write(Bus *bus, const WORD word) {
    bus->data = word;
}

WORD data_bus_read(const Bus *bus) {
    // banco->rMBR = barramento->dados;
    // rMBR_load(banco, barramento->dados);
    return bus->data;
}

void control_bus_set(Bus *bus, const CONTROL_BUS_FUNCTION func) {
    bus->control = func;
}
