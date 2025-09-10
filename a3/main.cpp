#include "Simulator.h"

/**
 * Main function - entry point for memory allocation simulation
 * Creates and runs simulation comparing first-fit vs best-fit allocation strategies
 */
int main() {
    // Create simulator instance
    Simulator simulator;
    
    // Run the complete simulation
    simulator.runSimulation();
    
    // Print performance comparison results
    simulator.printResults();
    
    return 0;
}