include ../Makefile.inc

OBJECTS = main.o FitSine.o Data.o

default: $(OBJECTS)
	$(CPP) -o main $(OBJECTS) $(CLIBS)

clean:
	rm -f main $(OBJECTS)
