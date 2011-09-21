CC=g++
CFLAGS= -Wall -DUSE_BOOST -L$(BOOST_LIB) -lboost_regex
SOURCES=Regexp.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE = Regexp

run: all
	./$(EXECUTABLE)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	- rm $(OBJECTS) $(EXECUTABLE)

.PHONY: clean

#g++ Regexp.cpp -o Regexp -L/opt/local/lib/ -lboost_regex -DUSE_BOOST
