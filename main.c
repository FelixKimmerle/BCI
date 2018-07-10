#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"
int main(int argc, const char *argv[])
{
  initVM();
  Chunk chunk;
  initChunk(&chunk);
  int con = addConstant(&chunk, 1.6);
  writeChunk(&chunk, OP_CONSTANT, 1);
  writeChunk(&chunk, con, 1);
  for (int i = 0; i < 10; i++)
  {
    writeConstant(&chunk, 1.3, 123);
  }
  writeChunk(&chunk, OP_RETURN, 123);
  disassembleChunk(&chunk, "test chunk");
  interpret(&chunk);
  freeChunk(&chunk);
  freeVM();
  return 0;
}
