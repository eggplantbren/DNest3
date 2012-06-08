# Compiler settings
CPP = g++
#CFLAGS = -O0 -g -Wall -Wextra -ansi -pedantic# Development
CFLAGS = -O2 -Wall -Wextra -ansi -pedantic -DNDEBUG# Optimized
CLIBS = -ldnest3 -lgsl -lgslcblas -lboost_thread -L../../lib
INCLUDES = ../../include

# The default build rule
.cpp.o:
	$(CPP) $(CFLAGS) -I$(INCLUDES) -o $*.o -c $*.cpp

default: main.o $(OBJECTS)
	$(CPP) -o main main.o $(OBJECTS) $(CLIBS)

clean:
	rm -f main main.o $(OBJECTS) *.pyc

