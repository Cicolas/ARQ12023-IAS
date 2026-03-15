#include "./uc.h"

UC *UC_create(void) {
    UC *uc = malloc(sizeof(UC));
    uc->pipeline = pipeline_create();
    uc->pipeline->flags |= PIPELINE_FLUSH;

    return uc;
}

void UC_free(UC *uc) {
    pipeline_free(uc->pipeline);
    free(uc);
}

void UC_tick(const UC *uc, ALU *alu, RegistersBank *banco, Bus *barramento, Memory *memoria) {
    // TODO: remove static count
    static int count = 0;
    printf("\n");
    PRINT("Tick! (%i)", count++);

    if (uc->pipeline->flags & PIPELINE_FLUSH) {
        pipeline_flush(uc->pipeline);
        PRINT("Empty Pipeline!");
        uc->pipeline->f_search_instruction = search_instruction;
    }

    static WORD p4_MBR = 0,
                p4_MAR = 0;

    if (uc->pipeline->f_write_results) {
        PRINT("-- Results Write --");
        pipeline_write_results(uc->pipeline, p4_MBR, p4_MAR, banco, barramento, memoria, alu);
        if (uc->pipeline->flags & PIPELINE_FLUSH || uc->pipeline->flags & STOP) return;
    }

    static WORD p3_IR  = 0,
                p3_MAR = 0,
                p3_MBR = 0;

    if (uc->pipeline->f_execute && !uc->pipeline->f_write_results) {
        PRINT("-- Execution --");
        PRINT("p3_IR: (%s)", optoa(p3_IR));
        PRINT("p3_MAR: (%ld)", p3_MAR);
        PRINT("p3_MBR: (%ld)", p3_MBR);
        PRINT("rAC: (%ld)", banco->rAC);
        pipeline_execute(uc->pipeline, p3_IR, p3_MAR, p3_MBR, &p4_MAR, &p4_MBR, banco, alu);

        if (uc->pipeline->flags & PIPELINE_FLUSH || uc->pipeline->flags & STOP) return;
    }

    static WORD p2_IR  = 0,
                p2_MAR = 0;

    if (uc->pipeline->f_search_operands && !uc->pipeline->f_execute && !pipeline_check_depency(uc->pipeline, p2_MAR, STOR)) {
        PRINT("-- Operands Search --");
        PRINT("rPC: %s", optoa(p2_IR));
        pipeline_search_operands(uc->pipeline, p2_IR, p2_MAR, &p3_IR, &p3_MAR, &p3_MBR, banco, barramento, memoria);
    }

    static WORD p1_MBR = 0;

    if (uc->pipeline->f_decode && !uc->pipeline->f_search_operands) {
        PRINT("-- Decode --");
        pipeline_decode(uc->pipeline, p1_MBR, &p2_IR, &p2_MAR, banco);
        PRINT("rPC: %s", optoa(banco->rIR));
    }

    if (uc->pipeline->f_search_instruction && !uc->pipeline->f_decode && !pipeline_check_depency(uc->pipeline, rPC_read(banco), STOR_LR)) {
        PRINT("-- Instruction Search --");
        PRINT("rPC: %ld", banco->rPC);
        pipeline_search_instruction(uc->pipeline, &p1_MBR, banco, barramento, memoria);
    }

    if (uc->pipeline->flags & STOR_EXECUTED) {
        pipeline_pop_dependency(uc->pipeline);
        uc->pipeline->flags &= ~STOR_EXECUTED;
    }
}
