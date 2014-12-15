OBJECTS=$(subst .cpp,.o,$(SOURCES))

all: $(EXECUTABLE)
			./$(EXECUTABLE)

edit: 
	vim $(SOURCES)

tidy:
	clang-tidy -fix $(SOURCES) -- $(CPPFLAGS)

format:
	clang-format -i $(SOURCES)

$(EXECUTABLE): $(OBJECTS)
	    $(CXX) $(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LDLIBS) 

depend: .depend

.depend: $(SOURCES)
	    $(RM) ./.depend
			$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	    $(RM) $(OBJECTS) $(EXECUTABLE)

-include .depend
