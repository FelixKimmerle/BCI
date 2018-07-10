#include <stdio.h>
#include "common.h"
#include "vm.h"
#include "debug.h"

VM vm;
static void resetStack()
{
    vm.stackTop = vm.stack;
}
void initVM()
{
    resetStack();
}

void freeVM()
{
}

void push(Value value)
{
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop()
{
    vm.stackTop--;
    return *vm.stackTop;
}

static InterpretResult run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_LONG_CONSTANT() (vm.chunk->constants.values[(READ_BYTE() | (READ_BYTE() << 8))])

    for (;;)
    {
#ifdef DEBUG_TRACE_EXECUTION
        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE())
        {
        case OP_RETURN:
        {
            return INTERPRET_OK;
        }
        case OP_CONSTANT:
        {
            Value constant = READ_CONSTANT();
            printValue(constant);
            printf("\n");
            break;
        }
        case OP_CONSTANT_LONG:
        {
            Value constant = READ_LONG_CONSTANT();
            printValue(constant);
            printf("\n");
            break;
        }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_LONG_CONSTANT
}

InterpretResult interpret(Chunk *chunk)
{
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}