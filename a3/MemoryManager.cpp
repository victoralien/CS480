#include "MemoryManager.h"
#include <iostream>

MemoryManager::MemoryManager(AllocationStrategy allocStrategy) 
    : strategy(allocStrategy) {
    // Initialize with one large free block covering all memory
    head = new MemoryBlock(0, TOTAL_UNITS, -1);
    totalAllocations = 0;
    deniedAllocations = 0;
    totalNodesTraversed = 0;
    totalFragments = 0;
    fragmentMeasurements = 0;
}

MemoryManager::~MemoryManager() {
    // Clean up entire linked list
    MemoryBlock* current = head;
    while (current != nullptr) {
        MemoryBlock* temp = current;
        current = current->next;
        delete temp;
    }
}

int MemoryManager::allocate_mem(int process_id, int num_units) {
    if (num_units <= 0) {
        return -1;
    }
    
    // Delegate to appropriate allocation strategy
    switch (strategy) {
        case FIRST_FIT:
            return allocateFirstFit(process_id, num_units);
        case BEST_FIT:
            return allocateBestFit(process_id, num_units);
        default:
            return -1;
    }
}

int MemoryManager::allocateFirstFit(int processId, int numUnits) {
    MemoryBlock* current = head;
    int nodesTraversed = 0;
    
    // Search for first block that can accommodate the request
    while (current != nullptr) {
        nodesTraversed++;
        
        // Check if this is a free block with sufficient size
        if (current->processId == -1 && current->size >= numUnits) {
            if (current->size == numUnits) {
                // Exact fit - just allocate the entire block
                current->processId = processId;
            } else {
                // Split the block - create new free block for remainder
                MemoryBlock* newBlock = new MemoryBlock(
                    current->startUnit + numUnits, 
                    current->size - numUnits, 
                    -1);
                newBlock->next = current->next;
                current->size = numUnits;
                current->processId = processId;
                current->next = newBlock;
            }
            
            // Update statistics
            totalAllocations++;
            totalNodesTraversed += nodesTraversed;
            
            return nodesTraversed;
        }
        current = current->next;
    }
    
    // No suitable block found
    deniedAllocations++;
    return -1;
}

int MemoryManager::allocateBestFit(int processId, int numUnits) {
    MemoryBlock* current = head;
    MemoryBlock* bestFit = nullptr;
    int nodesTraversed = 0;
    int bestSize = TOTAL_UNITS + 1; // Initialize to impossibly large value
    
    // First pass: find the smallest block that can accommodate the request
    while (current != nullptr) {
        nodesTraversed++;
        
        if (current->processId == -1 && current->size >= numUnits) {
            if (current->size < bestSize) {
                bestFit = current;
                bestSize = current->size;
            }
        }
        current = current->next;
    }
    
    // If no suitable block found
    if (bestFit == nullptr) {
        deniedAllocations++;
        return -1;
    }
    
    // Allocate the best fit block
    if (bestFit->size == numUnits) {
        // Exact fit
        bestFit->processId = processId;
    } else {
        // Split the block
        MemoryBlock* newBlock = new MemoryBlock(
            bestFit->startUnit + numUnits,
            bestFit->size - numUnits, 
            -1);
        newBlock->next = bestFit->next;
        bestFit->size = numUnits;
        bestFit->processId = processId;
        bestFit->next = newBlock;
    }
    
    // Update statistics
    totalAllocations++;
    totalNodesTraversed += nodesTraversed;
    
    return nodesTraversed;
}

int MemoryManager::deallocate_mem(int process_id) {
    MemoryBlock* current = head;
    MemoryBlock* prev = nullptr;
    
    // Search for the block allocated to this process
    while (current != nullptr) {
        if (current->processId == process_id) {
            // Mark block as free
            current->processId = -1;
            
            // Try to merge with next block if it's also free
            if (current->next != nullptr && current->next->processId == -1) {
                MemoryBlock* nextBlock = current->next;
                current->size += nextBlock->size;
                current->next = nextBlock->next;
                delete nextBlock;
            }
            
            // Try to merge with previous block if it's also free
            if (prev != nullptr && prev->processId == -1) {
                prev->size += current->size;
                prev->next = current->next;
                delete current;
            }
            
            return 1; // Success
        }
        prev = current;
        current = current->next;
    }
    
    return -1; // Process not found
}

int MemoryManager::fragment_count() {
    MemoryBlock* current = head;
    int count = 0;
    
    // Count free blocks of size 1 or 2 units
    while (current != nullptr) {
        if (current->processId == -1 && (current->size == 1 || current->size == 2)) {
            count++;
        }
        current = current->next;
    }
    
    return count;
}

void MemoryManager::updateFragmentStats() {
    totalFragments += fragment_count();
    fragmentMeasurements++;
}

double MemoryManager::getAvgExternalFragments() const {
    if (fragmentMeasurements > 0) {
        return static_cast<double>(totalFragments) / fragmentMeasurements;
    }
    return 0.0;
}

double MemoryManager::getAvgNodesTraversed() const {
    if (totalAllocations > 0) {
        return static_cast<double>(totalNodesTraversed) / totalAllocations;
    }
    return 0.0;
}

double MemoryManager::getPercentageDenied() const {
    int totalRequests = totalAllocations + deniedAllocations;
    if (totalRequests > 0) {
        return (static_cast<double>(deniedAllocations) / totalRequests) * 100.0;
    }
    return 0.0;
}

void MemoryManager::printMemoryList() const {
    MemoryBlock* current = head;
    std::cout << "Memory Layout: ";
    while (current != nullptr) {
        std::cout << "[" << current->startUnit << "-" 
                  << current->startUnit + current->size - 1 << ": " 
                  << (current->processId == -1 ? "FREE" : "ALLOC") << "] ";
        current = current->next;
    }
    std::cout << std::endl;
}