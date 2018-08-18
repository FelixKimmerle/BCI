#include <stdlib.h>
#include <stdio.h>
#include "chunk.h"
#include "memory.h"
#include "value.h"

void initChunk(Chunk *chunk)
{
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->linecount = 0;
    chunk->linecapacity = 0;
    chunk->linecounter = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk *chunk)
{
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->linecapacity);
    FREE_ARRAY(int, chunk->linecounter, chunk->linecapacity);
    freeValueArray(&chunk->constants);

    initChunk(chunk);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line)
{
    if (chunk->capacity < chunk->count + 1)
    {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(chunk->code, uint8_t,
                                 oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;


    if (chunk->linecount != 0 && chunk->lines[chunk->linecount - 1] == line)
    {
        chunk->linecounter[chunk->linecount - 1]++;
    }
    else
    {
        if (chunk->linecapacity < chunk->linecount + 1)
        {
            int oldCapacity = chunk->linecapacity;
            chunk->linecapacity = GROW_CAPACITY(oldCapacity);
            chunk->linecounter = GROW_ARRAY(chunk->linecounter, int, oldCapacity, chunk->linecapacity);
            chunk->lines = GROW_ARRAY(chunk->lines, int, oldCapacity, chunk->linecapacity);

        }
        chunk->lines[chunk->linecount] = line;
        chunk->linecounter[chunk->linecount] = 1;
        chunk->linecount++;
    }
}

bool writeConstant(Chunk *chunk, Value value, int line)
{
    writeValueArray(&chunk->constants, value);
    if (chunk->constants.count <= UINT8_MAX)
    {
        writeChunk(chunk, OP_CONSTANT, line);
        writeChunk(chunk, (chunk->constants.count - 1), line);
        return true;
    }
    else if (chunk->constants.count <= UINT16_MAX)
    {
        writeChunk(chunk, OP_CONSTANT_LONG, line);
        uint8_t a = (chunk->constants.count - 1) & 0xFF;
        uint8_t b = (chunk->constants.count - 1) >> 8;
        writeChunk(chunk, a, line);
        writeChunk(chunk, b, line);
        return true;
    }
    else
    {
        return false;
    }
}
