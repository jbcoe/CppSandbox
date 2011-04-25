CC=g++-mp-4.6
CFLAGS=-std=c++0x
SOURCES=MoveSemantics.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE = MoveSemantics

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

