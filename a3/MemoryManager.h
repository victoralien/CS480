#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "MemoryBlock.h"

/**
 * Memory allocation strategies
 */
enum AllocationStrategy {
    FIRST_FIT,
    BEST_FIT
};

/**
 * MemoryManager class implements memory allocation/deallocation using linked lists.
 * Supports both first-fit and best-fit allocation strategies.
 * Memory size: 256 KB divided into 128 units of 2 KB each.
 */
class MemoryManager {
private:
    static const int TOTAL_UNITS = 128;  // 256 KB / 2 KB = 128 units
    
    MemoryBlock* head;              // Head of linked list
    AllocationStrategy strategy;    // Current allocation strategy
    int totalAllocations;          // Total successful allocations
    int deniedAllocations;         // Total denied allocations
    int totalNodesTraversed;       // Sum of nodes traversed for all allocations
    int totalFragments;            // Sum of fragment counts across measurements
    int fragmentMeasurements;      // Number of fragment measurements taken

    /**
     * Internal helper methods for different allocation strategies
     */
    int allocateFirstFit(int processId, int numUnits);
    int allocateBestFit(int processId, int numUnits);

public:
    /**
     * Constructor - initializes memory manager with specified strategy
     * @param allocStrategy The allocation strategy to use (FIRST_FIT or BEST_FIT)
     */
    MemoryManager(AllocationStrategy allocStrategy);
    
    /**
     * Destructor - cleans up linked list
     */
    ~MemoryManager();
    
    // Required interface functions as specified in assignment
    /**
     * Allocates num_units units of memory to a process
     * @param process_id ID of the process requesting memory
     * @param num_units Number of memory units requested
     * @return Number of nodes traversed if successful, -1 if failed
     */
    int allocate_mem(int process_id, int num_units);
    
    /**
     * Deallocates memory allocated to a process
     * @param process_id ID of the process whose memory should be deallocated
     * @return 1 if successful, -1 if process not found
     */
    int deallocate_mem(int process_id);
    
    /**
     * Counts the number of external fragments (holes of size 1 or 2 units)
     * @return Number of small fragments
     */
    int fragment_count();
    
    // Statistics and utility functions
    /**
     * Updates fragment statistics by adding current fragment count
     */
    void updateFragmentStats();
    
    /**
     * Gets average number of external fragments across all measurements
     * @return Average fragment count
     */
    double getAvgExternalFragments() const;
    
    /**
     * Gets average number of nodes traversed per allocation
     * @return Average nodes traversed
     */
    double getAvgNodesTraversed() const;
    
    /**
     * Gets percentage of allocation requests that were denied
     * @return Percentage of denied requests
     */
    double getPercentageDenied() const;
    
    /**
     * Prints current memory layout for debugging
     */
    void printMemoryList() const;
    
    /**
     * Gets the current allocation strategy
     * @return Current strategy
     */
    AllocationStrategy getStrategy() const { return strategy; }
};

#endif