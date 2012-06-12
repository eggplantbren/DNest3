# Compiler settings
CPP = g++
# Optimized
CFLAGS = -O2 -Wall -Wextra -ansi -pedantic -DNDEBUG -DDNest3_No_Boost
CLIBS = -ldnest3 -lgsl -lgslcblas -L../../lib
INCLUDES = ../../include

# The default build rule
.cpp.o:
	$(CPP) $(CFLAGS) -I$(INCLUDES) -o $*.o -c $*.cpp

default: main.o $(OBJECTS)
	$(CPP) -o main main.o $(OBJECTS) $(CLIBS)

clean:
	rm -f main main.o $(OBJECTS) *.pyc

