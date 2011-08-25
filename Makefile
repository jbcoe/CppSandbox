CC=g++-mp-4.6
CFLAGS=-O0 -std=c++0x 
SOURCES=puzzle.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE = Puzzle

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	- rm $(OBJECTS) $(EXECUTABLE)

.PHONY: clean

