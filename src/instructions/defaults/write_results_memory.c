#include "write_results_memory.h"

void write_results_memory(RegistersBank *bank, Bus *bus, Memory *mem, WORD mask) {
    CONTROL_BUS_FUNCTION funcao;
    if (mask == MASK) {
        data_bus_write(bus, rMBR_read(bank));
        funcao = STORE;
    }
    else if (mask == LEFT_MASK) {
        data_bus_write(bus, (rMBR_read(bank) << 20) & LEFT_MASK);
        funcao = PARTIAL_STORE_L;
    }
    else if (mask == RIGHT_MASK) {
        data_bus_write(bus, rMBR_read(bank) & RIGHT_MASK);
        funcao = PARTIAL_STORE_R;
    }
    else { RAISE("mascara desconhecida! (%lx)", mask); }

    addr_bus_write(bus, rMAR_read(bank));
    control_bus_set(bus, funcao);
    mem_tick(mem, bus);
}
