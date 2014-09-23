OBJECTS=$(subst .cpp,.o,$(SOURCES))

all: $(EXECUTABLE)
			./$(EXECUTABLE)

edit: 
	vim $(SOURCES)

$(EXECUTABLE): $(OBJECTS)
	    $(CXX) $(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LDLIBS) 

depend: .depend

.depend: $(SOURCES)
	    $(RM) ./.depend
			$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	    $(RM) $(OBJECTS) $(EXECUTABLE)

-include .depend
