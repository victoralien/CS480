#include "MemoryBlock.h"

MemoryBlock::MemoryBlock(int start, int blockSize, int procId) 
    : startUnit(start), size(blockSize), processId(procId), next(nullptr) {
}

MemoryBlock::~MemoryBlock() {
    // Destructor - no dynamic cleanup needed for basic members
}