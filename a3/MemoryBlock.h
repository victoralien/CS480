#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

/**
 * MemoryBlock class represents a contiguous block of memory units.
 * Used in a linked list to track allocated and free memory blocks.
 */
class MemoryBlock {
public:
    int startUnit;      // Starting unit number of this block
    int size;           // Size of block in units
    int processId;      // Process ID (-1 for free blocks)
    MemoryBlock* next;  // Pointer to next block in linked list
    
    /**
     * Constructor for MemoryBlock
     * @param start Starting unit number
     * @param blockSize Size in memory units
     * @param procId Process ID (-1 for free blocks)
     */
    MemoryBlock(int start, int blockSize, int procId);
    
    /**
     * Destructor
     */
    ~MemoryBlock();
};

#endif