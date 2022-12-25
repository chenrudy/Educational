#!/bin/bash

TARGET = $(shell basename  $(CURDIR))
DEPEND = 

CC = g++
GD_FLAGS = -std=c++11 -pedantic-errors -Wall -Wextra -g
GC_FLAGS = -std=c++11 -pedantic-errors -Wall -Wextra -DNDEBUG -O3
OBJs_FLAGS = -c -fpic
LD_FLAGS = -L$(DEBUG_LIBSDIR) -Wl,-rpath=$(DEBUG_LIBSDIR)
INCLUDE_FLAG = -I../include 

.PHONY: all	test release debug clean

test:  
	$(CC) $(GD_FLAGS) $(INCLUDE_FLAG) -o $(TARGET).out $(TARGET).cpp $(TARGET)_test.cpp -fext-numeric-literals

clean:
	rm -f $(TARGET).out
	rm -f $(TARGET).hpp.gch
