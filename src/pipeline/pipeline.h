#ifndef PIPELINE_H_
#define PIPELINE_H_

#include <stdint.h>
#include "../defs/defs.h"
#include "../memory/memory.h"
#include "../bus/bus.h"
#include "../ula/ula.h"
#include "../utils.h"
#include "../instructions/defaults/decode.h"

#define DEPENDENCY_QUEUE_SIZE 4

#define FUN_SEARCH_INSTRUCTION(name) \
    void (*name)(RegistersBank *bank, Bus *barramento, Memory *memoria)

#define FUN_DECODE(name) \
    void (*name)(RegistersBank *bank)

#define FUN_SEARCH_OPERANDS(name) \
    void (*name)(RegistersBank *bank, Bus *barramento, Memory *memoria)

#define FUN_EXECUTE(name) \
    void (*name)(int interaction, RegistersBank *bank, ALU *alu, PipelineFlag *flags)

#define FUN_WRITE_RESULTS(name) \
    void (*name)(RegistersBank *bank, Bus *barramento, Memory *memoria, ALU *alu, PipelineFlag *flags)

typedef struct {
    short time;
    FUN_SEARCH_OPERANDS(f_search_operands);
    FUN_EXECUTE(f_execute);
    FUN_WRITE_RESULTS(f_write_results);
} InstructionConfig;

typedef struct {
    INSTRUCTION op;
    ARGUMENT position;
} DependencyNode;

typedef enum {
    STOR,
    STOR_LR
} DependencyType;

typedef struct {
    PipelineFlag flags;

    InstructionConfig instructions[OP_STOR + 1];
    uint8_t execution_cycle;
    DependencyNode dependency_queue[DEPENDENCY_QUEUE_SIZE];

    FUN_SEARCH_INSTRUCTION(f_search_instruction);
    FUN_DECODE(f_decode);
    FUN_SEARCH_OPERANDS(f_search_operands);
    FUN_EXECUTE(f_execute);
    FUN_WRITE_RESULTS(f_write_results);
} Pipeline;

Pipeline *pipeline_create(void);
void pipeline_free(Pipeline *pipeline);

void pipeline_insert_op_time(Pipeline *pipeline, INSTRUCTION op, short time);
InstructionConfig pipeline_get_instruction(const Pipeline *pipeline, INSTRUCTION op);
void pipeline_set_instruction(Pipeline *pipeline, INSTRUCTION op,
                            FUN_SEARCH_OPERANDS(f_search_operands),
                            FUN_EXECUTE(f_execute),
                            FUN_WRITE_RESULTS(f_write_results));

void pipeline_search_instruction(Pipeline *pipeline, WORD *p1_MBR, RegistersBank *banco, Bus *barramento, Memory *memoria);
void pipeline_decode(Pipeline *pipeline, WORD p1_MBR, WORD *p2_IR, WORD *p2_MAR, RegistersBank *banco);
void pipeline_search_operands(Pipeline *pipeline, WORD p2_IR, WORD p2_MAR, WORD *p3_IR, WORD *p3_MAR, WORD *p3_MBR, RegistersBank *banco, Bus *barramento, Memory *memoria);
void pipeline_execute(Pipeline *pipeline, WORD p3_IR, WORD p3_MAR, WORD p3_MBR, WORD *p4_MAR, WORD *p4_MBR, RegistersBank *banco, ALU *ula);
void pipeline_write_results(Pipeline *pipeline, WORD p4_MBR, WORD p4_MAR, RegistersBank *banco, Bus *barramento, Memory *memoria, ALU *ula);
void pipeline_flush(Pipeline *pipeline);
void pipeline_enqueue_dependency(Pipeline *pipeline, INSTRUCTION op, ARGUMENT posicao);
// void pipeline_pop_dep(Pipeline *pipeline, INSTRUCTION op, ARGUMENT posicao);
void pipeline_pop_dependency(Pipeline *pipeline);
void pipeline_unshift_dependency(Pipeline *pipeline);
bool pipeline_check_depency(Pipeline *pipeline, ARGUMENT posicao, DependencyType tipo);

#endif
