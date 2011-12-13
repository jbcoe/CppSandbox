OBJECTS=$(SOURCES:.cpp=.o)

run: all
	./$(EXECUTABLE)

all: $(SOURCES) $(EXECUTABLE)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

clean:
	- rm $(OBJECTS) $(EXECUTABLE)

.PHONY: clean

