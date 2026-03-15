#include "search_instruction.h"

void search_instruction(RegistersBank *bank, Bus *bus, Memory *mem) {
    if (bank->rIBR == 0) {
        // MAR <- PC
        rMAR_load(bank, rPC_read(bank), RIGHT_MASK);

        // bus.endereco <- MAR
        addr_bus_write(bus, rMAR_read(bank));
        // bus.dados <- M(bus.endereco)
        control_bus_set(bus, LOAD);
        mem_tick(mem, bus);
        // MBT <- bus.dados
        rMBR_load(bank, data_bus_read(bus));
    }
}