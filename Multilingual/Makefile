FORTRAN=gfortran-mp-4.6
CC=gcc

SOURCES=useDoubler.c           
EXECUTABLE=Multilingual

OBJECTS=$(SOURCES:.c=.o) double.o

run: all
	./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

double.o: double.f95
	$(FORTRAN) double.f95 -c -o double.o
all: $(SOURCES) $(EXECUTABLE)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	- rm $(OBJECTS) $(EXECUTABLE)

.PHONY: clean

