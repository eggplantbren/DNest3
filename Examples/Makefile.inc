# Compiler settings
CPP = g++
# CFLAGS = -O0 -g -Wall -Wextra -ansi -pedantic  # Development
CFLAGS = -O2 -Wall -Wextra -ansi -pedantic -DNDEBUG  # Optimized
CLIBS = -ldnest3 -lgsl -lgslcblas -L../../build
INCLUDES = ../../src

# The default build rule
.cpp.o:
	$(CPP) $(CFLAGS) -I$(INCLUDES) -o $*.o -c $*.cpp
