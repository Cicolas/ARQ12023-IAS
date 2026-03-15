#include "decode.h"

void decode(RegistersBank *bank) {
    if (bank->rIBR == 0) {
        if ((bank->rMBR & LEFT_MASK) == 0) {
            rIR_load(bank, bank->rMBR, RIGHT_MASK);
            rMAR_load(bank, bank->rMBR, RIGHT_MASK);

            rPC_increment(bank);
        } else {
            rIR_load(bank, bank->rMBR, LEFT_MASK);
            rMAR_load(bank, bank->rMBR, LEFT_MASK);

            rIBR_load(bank, bank->rMBR);
        }
    } else {
        rIR_load(bank, bank->rIBR, RIGHT_MASK);
        rMAR_load(bank, bank->rIBR, RIGHT_MASK);

        rIBR_reset(bank);
        rPC_increment(bank);
    }
}