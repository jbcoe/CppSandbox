CC=g++-mp-4.6
CFLAGS=-O0 -std=c++0x 
LDFLAGS=-L/opt/local/lib/ -lboost_signals
SOURCES=BoostSignals.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE = BoostSignals

run: all
	./$(EXECUTABLE)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	- rm $(OBJECTS) $(EXECUTABLE)

.PHONY: clean

