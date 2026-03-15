#include "./pipeline.h"

Pipeline *pipeline_create(void) {
    Pipeline *pipeline = malloc(sizeof(Pipeline));

    pipeline->execution_cycle = 0;
    pipeline->flags = 0;

    for (int i = 0; i < DEPENDENCY_QUEUE_SIZE; ++i)
        pipeline->dependency_queue[i].op = 0;

    pipeline->f_search_instruction = NULL;
    pipeline->f_decode = NULL;
    pipeline->f_search_operands = NULL;
    pipeline->f_execute = NULL;
    pipeline->f_write_results = NULL;

    return pipeline;
}

void pipeline_free(Pipeline *pipeline) {
    free(pipeline);
}

void pipeline_insert_op_time(Pipeline *pipeline, INSTRUCTION op, const short time) {
    if (op == OP_EXIT) op = 0;

    pipeline->instructions[op].time = time;

    if (op == OP_JUMP_COND_L)
        pipeline->instructions[OP_JUMP_COND_R].time = time;
    else if (op == OP_JUMP_COND_R)
        pipeline->instructions[OP_JUMP_COND_L].time = time;

    if (op == OP_JUMP_L)
        pipeline->instructions[OP_JUMP_R].time = time;
    else if (op == OP_JUMP_R)
        pipeline->instructions[OP_JUMP_L].time = time;

    if (op == OP_STOR_L)
        pipeline->instructions[OP_STOR_R].time = time;
}

InstructionConfig pipeline_get_instruction(const Pipeline *pipeline, INSTRUCTION op) {
    if (op == OP_EXIT) op = 0;
    return pipeline->instructions[op];
}

void pipeline_set_instruction(
        Pipeline *pipeline, INSTRUCTION op, FUN_SEARCH_OPERANDS(f_search_operands), FUN_EXECUTE(f_execute), FUN_WRITE_RESULTS(f_write_results)) {
    if (op == OP_EXIT) op = 0;

    pipeline->instructions[op].f_search_operands = f_search_operands;
    pipeline->instructions[op].f_execute = f_execute;
    pipeline->instructions[op].f_write_results = f_write_results;
}

void pipeline_search_instruction(
        Pipeline *pipeline, WORD *p1_MBR, RegistersBank *banco, Bus *barramento, Memory *memoria) {

    pipeline->f_search_instruction(banco, barramento, memoria);

    // TODO: COLOCAR MASCARA NA BUSCA INSTRUCTION
    if (pipeline->flags & LOAD_RIGHT) {
        rMBR_load(banco, rMBR_read(banco) & RIGHT_MASK);
        pipeline->flags &= ~LOAD_RIGHT;
    }

    *p1_MBR = banco->rMBR;

    pipeline->f_decode = decode;
}

void pipeline_decode(
        Pipeline *pipeline, WORD p1_MBR, WORD *p2_IR, WORD *p2_MAR, RegistersBank *banco) {

    WORD pc_execucao = banco->rPC;
    // check in
    banco->rMBR = p1_MBR;

    pipeline->f_decode(banco);
    pipeline->f_decode = NULL;

    if (banco->rIR == OP_STOR_R && rMAR_read(banco) == pc_execucao) {
        pipeline->flags |= LOAD_RIGHT;
        rIBR_reset(banco);
    }

    if (banco->rIR == OP_STOR_R || banco->rIR == OP_STOR_L) {
        pipeline_enqueue_dependency(pipeline, OP_STOR_R, banco->rMAR);
    }

    // check out
    *p2_IR  = banco->rIR;
    *p2_MAR = banco->rMAR;

    pipeline->f_search_operands = pipeline_get_instruction(pipeline, banco->rIR).f_search_operands;
}

void pipeline_search_operands(
        Pipeline *pipeline, WORD p2_IR, WORD p2_MAR, WORD *p3_IR, WORD *p3_MAR, WORD *p3_MBR, RegistersBank *banco, Bus *barramento, Memory *memoria) {

    banco->rMAR = p2_MAR;

    pipeline->f_search_operands(banco, barramento, memoria);
    pipeline->f_search_operands = NULL;

    *p3_IR = p2_IR;
    *p3_MBR = banco->rMBR;
    *p3_MAR = banco->rMAR;

    pipeline->f_execute = pipeline_get_instruction(pipeline, p2_IR).f_execute;
}

void pipeline_execute(
        Pipeline *pipeline, WORD p3_IR, WORD p3_MAR, WORD p3_MBR, WORD *p4_MAR, WORD *p4_MBR, RegistersBank *banco, ALU *ula) {

    banco->rMAR = p3_MAR;
    banco->rMBR = p3_MBR;

    pipeline->f_execute(pipeline->execution_cycle++, banco, ula, &pipeline->flags);

    if (pipeline->flags & STOR_EXECUTED) {
        PRINT("dependencia inclusa! (OP_STOR %ld)", rMAR_read(banco));
        pipeline_enqueue_dependency(pipeline, OP_STOR, rMAR_read(banco));
    }

    InstructionConfig inst = pipeline_get_instruction(pipeline, p3_IR);
    if (pipeline->execution_cycle >= inst.time) {
        pipeline->execution_cycle = 0;
        pipeline->f_execute = NULL;

        *p4_MAR = banco->rMAR;
        *p4_MBR = banco->rMBR;

        pipeline->f_write_results = inst.f_write_results;
    }
}

void pipeline_write_results(
        Pipeline *pipeline, WORD p4_MBR, WORD p4_MAR, RegistersBank *banco, Bus *barramento, Memory *memoria, ALU *ula) {

    banco->rMBR = p4_MBR;
    banco->rMAR = p4_MAR;

    pipeline->f_write_results(banco, barramento, memoria, ula, &pipeline->flags);
    pipeline->f_write_results = NULL;

    if (pipeline->flags & STOR_PARTIALLY_FINISHED) {
        pipeline->flags &= ~STOR_PARTIALLY_FINISHED;
        pipeline_unshift_dependency(pipeline);
    }
}

void pipeline_flush(Pipeline *pipeline) {
    pipeline->execution_cycle = 0;

    pipeline->f_search_instruction = NULL;
    pipeline->f_decode = NULL;
    pipeline->f_search_operands = NULL;
    pipeline->f_execute = NULL;
    pipeline->f_write_results = NULL;

    pipeline->flags &= ~PIPELINE_FLUSH;
}

void pipeline_enqueue_dependency(Pipeline *pipeline, INSTRUCTION op, ARGUMENT posicao) {
    for (int i = 0; i < DEPENDENCY_QUEUE_SIZE; i++) {
        if (pipeline->dependency_queue[i].op == 0) {
            pipeline->dependency_queue[i].op = op;
            pipeline->dependency_queue[i].position = (int16_t)posicao;
            return;
        }
    }

    WARN("fila dependencias cheia!");
}

void pipeline_pop_dependency(Pipeline *pipeline) {
    // int8_t pos;
    // for (
    //     pos = FILA_DEPENDENCIA_SIZE - 1;
    //     pos >= 0 && (pipeline->fila_dependencia[pos].op != op || pipeline->fila_dependencia[pos].posicao != posicao);
    //     pos--
    // );

    // if (pos < 0) return;

    // for (int i = pos; i < FILA_DEPENDENCIA_SIZE - 1; i++) {
    //     pipeline->fila_dependencia[i] = pipeline->fila_dependencia[i+1];
    //     pipeline->fila_dependencia[i + 1] = (NoDependencia){
    //         .op = 0
    //     };
    // }

    for (int i = DEPENDENCY_QUEUE_SIZE - 1; i >= 0; i--) {
        if (pipeline->dependency_queue[i].op != 0) {
            PRINT("FILA DEPENDENCIA POP (%s %d)", optoa(pipeline->dependency_queue[i].op), pipeline->dependency_queue[i].position);
            pipeline->dependency_queue[i].op = 0;
            return;
        }
    }

    WARN("fila dependencia vazia");
}

void pipeline_unshift_dependency(Pipeline *pipeline) {
    for (int i = 0; i < DEPENDENCY_QUEUE_SIZE - 1; i++) {
        PRINT("fila dependencia unshift (%s %d)", optoa(pipeline->dependency_queue[i].op), pipeline->dependency_queue[i].position);
        pipeline->dependency_queue[i] = pipeline->dependency_queue[i + 1];
        pipeline->dependency_queue[i + 1] = (DependencyNode){
            .op = 0,
            .position = 0
        };
    }
}

bool pipeline_check_depency(Pipeline *pipeline, ARGUMENT posicao, DependencyType tipo) {
    for (int i = 0; i < DEPENDENCY_QUEUE_SIZE; i++) {
        DependencyNode no = pipeline->dependency_queue[i];

        if (no.op != 0 && no.position == posicao) {
            PRINT("dependencia encontrada! (%s %d)", optoa(no.op), no.position);
            if ((tipo == STOR    && no.op == OP_STOR) ||
                (tipo == STOR_LR && (no.op == OP_STOR_R || no.op == OP_STOR_L))) {
                return true;
            }
        }
    }

    return false;
}