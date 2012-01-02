all: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) || echo Compilation failure expected

clean:

.PHONY: all clean
