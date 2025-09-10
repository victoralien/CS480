#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "MemoryManager.h"

/**
 * Simulator class implements the request generation and statistics reporting components.
 * Generates 10,000 allocation/deallocation requests and compares first-fit vs best-fit performance.
 */
class Simulator {
private:
    static const int NUM_REQUESTS = 10000;  // Total number of requests to generate
    static const int MIN_REQUEST = 3;       // Minimum units per request
    static const int MAX_REQUEST = 10;      // Maximum units per request
    
    MemoryManager* firstFitManager;    // Memory manager using first-fit strategy
    MemoryManager* bestFitManager;     // Memory manager using best-fit strategy
    int* allocatedProcesses;           // Array tracking allocated process IDs
    int numAllocated;                  // Current number of allocated processes
    int maxAllocated;                  // Maximum possible allocated processes

public:
    /**
     * Constructor - initializes both memory managers and process tracking
     */
    Simulator();
    
    /**
     * Destructor - cleans up allocated resources
     */
    ~Simulator();
    
    /**
     * Runs the complete simulation with 10,000 requests
     */
    void runSimulation();
    
    /**
     * Prints final performance statistics for both allocation strategies
     */
    void printResults();
    
private:
    /**
     * Generates a single allocation or deallocation request
     * @param requestNumber Sequential request number (used as process ID for allocations)
     */
    void generateRequest(int requestNumber);
    
    /**
     * Handles memory allocation for both managers
     * @param processId Process ID requesting memory
     * @param numUnits Number of memory units requested
     */
    void allocateMemory(int processId, int numUnits);
    
    /**
     * Handles memory deallocation for both managers
     * Randomly selects a currently allocated process to deallocate
     */
    void deallocateMemory();
};

#endif