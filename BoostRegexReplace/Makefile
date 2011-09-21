CC=g++-mp-4.6
CFLAGS=-O0 -std=c++0x 
SOURCES=BoostRegexReplace.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE = BoostRegexReplace
LDFLAGS = -L$(BOOST_LIB) -lboost_regex

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

