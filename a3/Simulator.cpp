#include "Simulator.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <fstream>

Simulator::Simulator() {
    // Initialize memory managers with different strategies
    firstFitManager = new MemoryManager(FIRST_FIT);
    bestFitManager = new MemoryManager(BEST_FIT);
    
    // Initialize process tracking array
    maxAllocated = NUM_REQUESTS;  // Upper bound on number of allocated processes
    allocatedProcesses = new int[maxAllocated];
    numAllocated = 0;
    
    // Seed random number generator
    std::srand(std::time(nullptr));
}

Simulator::~Simulator() {
    delete firstFitManager;
    delete bestFitManager;
    delete[] allocatedProcesses;
}

void Simulator::runSimulation() {
    std::cout << "Starting memory allocation simulation with " << NUM_REQUESTS << " requests..." << std::endl;
    std::cout << "Memory size: 256 KB (128 units of 2 KB each)" << std::endl;
    std::cout << "Request sizes: " << MIN_REQUEST << "-" << MAX_REQUEST << " units" << std::endl << std::endl;
    
    // Open file for time series data
    std::ofstream timeSeriesFile("fragmentation_data.txt");
    timeSeriesFile << "Request,FirstFit_Fragments,BestFit_Fragments,FirstFit_AvgNodes,BestFit_AvgNodes\n";
    
    // Generate and process all requests
    for (int i = 0; i < NUM_REQUESTS; i++) {
        generateRequest(i);
        
        // Record data every 100 requests for graphing
        if ((i + 1) % 100 == 0) {
            timeSeriesFile << (i + 1) << ","
                          << firstFitManager->getAvgExternalFragments() << ","
                          << bestFitManager->getAvgExternalFragments() << ","
                          << firstFitManager->getAvgNodesTraversed() << ","
                          << bestFitManager->getAvgNodesTraversed() << std::endl;
        }
        
        // Print progress every 1000 requests
        if ((i + 1) % 1000 == 0) {
            std::cout << "Processed " << (i + 1) << " requests..." << std::endl;
        }
    }
    
    timeSeriesFile.close();
    std::cout << "Simulation complete!" << std::endl;
    std::cout << "Time series data saved to fragmentation_data.txt" << std::endl << std::endl;
}

void Simulator::generateRequest(int requestNumber) {
    // 50% chance of allocation, 50% chance of deallocation
    // However, if no processes are allocated, force an allocation
    bool shouldAllocate = (std::rand() % 2 == 0) || (numAllocated == 0);
    
    if (shouldAllocate) {
        // Allocation request
        int processId = requestNumber;  // Use request number as unique process ID
        int numUnits = MIN_REQUEST + (std::rand() % (MAX_REQUEST - MIN_REQUEST + 1));
        allocateMemory(processId, numUnits);
    } else {
        // Deallocation request
        deallocateMemory();
    }
    
    // Update fragment statistics for both managers after each request
    firstFitManager->updateFragmentStats();
    bestFitManager->updateFragmentStats();
}

void Simulator::allocateMemory(int processId, int numUnits) {
    // Attempt allocation on both managers
    int ffResult = firstFitManager->allocate_mem(processId, numUnits);
    int bfResult = bestFitManager->allocate_mem(processId, numUnits);
    
    // Track process as allocated only if both allocations succeeded
    // This ensures both managers stay in sync
    if (ffResult > 0 && bfResult > 0) {
        if (numAllocated < maxAllocated) {
            allocatedProcesses[numAllocated] = processId;
            numAllocated++;
        }
    }
    
    // Handle synchronization issues - if one succeeded and other failed,
    // we need to deallocate the successful one to keep them in sync
    if (ffResult > 0 && bfResult <= 0) {
        firstFitManager->deallocate_mem(processId);
        std::cerr << "Warning: Best fit failed, rolling back first fit allocation for process " << processId << std::endl;
    } else if (ffResult <= 0 && bfResult > 0) {
        bestFitManager->deallocate_mem(processId);
        std::cerr << "Warning: First fit failed, rolling back best fit allocation for process " << processId << std::endl;
    }
}

void Simulator::deallocateMemory() {
    if (numAllocated > 0) {
        // Randomly select a process to deallocate
        int index = std::rand() % numAllocated;
        int processId = allocatedProcesses[index];
        
        // Remove from allocated processes array by shifting remaining elements
        for (int j = index; j < numAllocated - 1; j++) {
            allocatedProcesses[j] = allocatedProcesses[j + 1];
        }
        numAllocated--;
        
        // Deallocate from both managers
        int ffResult = firstFitManager->deallocate_mem(processId);
        int bfResult = bestFitManager->deallocate_mem(processId);
        
        // Check for synchronization issues
        if (ffResult != bfResult) {
            std::cerr << "Warning: Deallocation synchronization issue for process " << processId << std::endl;
        }
    }
}

void Simulator::printResults() {
    std::cout << std::fixed << std::setprecision(6);
    
    // Print First Fit results
    std::cout << "End of First Fit Allocation" << std::endl;
    std::cout << "Average External Fragments Each Request: " 
              << firstFitManager->getAvgExternalFragments() << std::endl;
    std::cout << "Average Nodes Transversed Each Allocation: " 
              << firstFitManager->getAvgNodesTraversed() << std::endl;
    std::cout << "Percentage Allocation Requests Denied Overall: " 
              << firstFitManager->getPercentageDenied() << "%" << std::endl << std::endl;
    
    // Print Best Fit results
    std::cout << "End of Best Fit Allocation" << std::endl;
    std::cout << "Average External Fragments Each Request: " 
              << bestFitManager->getAvgExternalFragments() << std::endl;
    std::cout << "Average Nodes Transversed Each Allocation: " 
              << bestFitManager->getAvgNodesTraversed() << std::endl;
    std::cout << "Percentage Allocation Requests Denied Overall: " 
              << bestFitManager->getPercentageDenied() << "%" << std::endl << std::endl;
    
    // Save results to file for Python graphing
    std::ofstream resultsFile("simulation_results.txt");
    resultsFile << std::fixed << std::setprecision(6);
    resultsFile << "FirstFit_Fragments: " << firstFitManager->getAvgExternalFragments() << std::endl;
    resultsFile << "FirstFit_Nodes: " << firstFitManager->getAvgNodesTraversed() << std::endl;
    resultsFile << "FirstFit_Denied: " << firstFitManager->getPercentageDenied() << std::endl;
    resultsFile << "BestFit_Fragments: " << bestFitManager->getAvgExternalFragments() << std::endl;
    resultsFile << "BestFit_Nodes: " << bestFitManager->getAvgNodesTraversed() << std::endl;
    resultsFile << "BestFit_Denied: " << bestFitManager->getPercentageDenied() << std::endl;
    resultsFile.close();
    
    std::cout << "Results saved to simulation_results.txt for graphing" << std::endl;
}