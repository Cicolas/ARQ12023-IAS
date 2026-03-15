#include "search_operands.h"

void search_operands(RegistersBank *bank, Bus *bus, Memory *mem) {
    // bus.endereco <- MAR
    addr_bus_write(bus, rMAR_read(bank));
    // bus.dados <- M(bus.endereco)
    control_bus_set(bus, LOAD);
    mem_tick(mem, bus);
    // MBT <- bus.dados
    rMBR_load(bank, data_bus_read(bus));
}