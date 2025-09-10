# Makefile
CXX      = g++
CXXFLAGS = -Wall -g
OBJS     = main.o xsh.o
TARGET   = dsh

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp xsh.h
	$(CXX) $(CXXFLAGS) -c main.cpp

xsh.o: xsh.cpp xsh.h
	$(CXX) $(CXXFLAGS) -c xsh.cpp

clean:
	rm -f $(OBJS) $(TARGET)
