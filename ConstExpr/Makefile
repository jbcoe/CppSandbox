CC=g++-mp-4.6
CFLAGS=-O0 -std=c++0x 
#-fno-elide-constructors
SOURCES=ConstExpr.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE = ../bin/ConstExpr

run: all
	./$(EXECUTABLE)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	- rm $(OBJECTS) $(EXECUTABLE)

.PHONY: clean

