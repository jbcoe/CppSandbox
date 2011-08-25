FC=gfortran
CC=gcc
CFLAGS=-arch i386
LDFLAGS=$(CFLAGS)

OBJECTS=double.o\
				useDoubler.o

EXECUTABLE = Doubler

.SUFFIXES: .f95 .o .c

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@

.f95.o:
	$(FC) $(FFLAGS) $< -c -o $@

clean:
	- rm $(OBJECTS) $(EXECUTABLE)

.PHONY: clean

