CC = gcc
CFLAGS = -Wall -Wextra -pthread -std=c11
TARGET = bots
SRC = bots.c

all: $(TARGET)

$(TARGET): $(SRC) bots.h
	$(CC) $(CFLAGS) -o $@ $(SRC)

clean: 
	rm -f $(TARGET) QUOTE.txt

.PHONY: all clean