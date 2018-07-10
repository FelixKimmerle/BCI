#include <stdio.h>

#include "debug.h"
#include "value.h"

void disassembleChunk(Chunk *chunk, const char *name)
{
    printf("== %s ==\n", name);

    for (int i = 0; i < chunk->count;)
    {
        i = disassembleInstruction(chunk, i);
    }
}
static int simpleInstruction(const char *name, int offset)
{
    printf("%s\n", name);
    return offset + 1;
}
static int constantInstruction(const char *name, Chunk *chunk,
                               int offset)
{
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}
static int longconstantInstruction(const char *name, Chunk *chunk,
                                   int offset)
{
    uint8_t a = chunk->code[offset + 1];
    uint8_t b = chunk->code[offset + 2];
    uint16_t constant = (b << 8) | a;
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 3;
}
int disassembleInstruction(Chunk *chunk, int offset)
{
    printf("%04d ", offset);
    if (offset > 0 && getLine(chunk, offset) == getLine(chunk, offset - 1))
    {
        printf("   | ");
    }
    else
    {
        printf("%4d ", getLine(chunk, offset));
    }

    uint8_t instruction = chunk->code[offset];
    switch (instruction)
    {
    case OP_CONSTANT:
        return constantInstruction("OP_CONSTANT", chunk, offset);
    case OP_RETURN:
        return simpleInstruction("OP_RETURN", offset);
    case OP_CONSTANT_LONG:
        return longconstantInstruction("OP_CONSTANT_LONG", chunk, offset);
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}

int getLine(Chunk *chunk, int offset)
{
    int y = 0;
    for (int x = 0; x < chunk->linecount; x++)
    {
        y += chunk->linecounter[x];
        if (y >= offset)
        {
            return chunk->lines[x];
        }
    }
}