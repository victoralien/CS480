# Makefile for Memory Allocation Simulation
# Assignment 3 - Operating Systems

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2
TARGET = sim

# Source files
SOURCES = main.cpp MemoryBlock.cpp MemoryManager.cpp Simulator.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = MemoryBlock.h MemoryManager.h Simulator.h

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
	@echo "Build complete. Executable: $(TARGET)"

# Compile source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run simulation
run: $(TARGET)
	./$(TARGET)

# Run simulation and generate graphs
graphs: $(TARGET)
	./$(TARGET)
	python3 generate_graphs.py

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -f simulation_results.txt fragmentation_data.txt
	rm -f *.png
	@echo "Clean complete."

# Clean everything including generated files
distclean: clean
	rm -f *.png *.txt
	@echo "Deep clean complete."

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build the simulation executable"
	@echo "  run       - Build and run the simulation"
	@echo "  graphs    - Build, run simulation, and generate graphs"
	@echo "  clean     - Remove object files and executable"
	@echo "  distclean - Remove all generated files"
	@echo "  help      - Show this help message"

# Declare phony targets
.PHONY: all run graphs clean distclean help
