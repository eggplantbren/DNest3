# Development
CFLAGS = -O0 -g -Wall -Wextra -ansi -pedantic

# Optimized
#CFLAGS = -O2 -Wall -Wextra -ansi -pedantic -DNDEBUG

LIBS = -ldnest3 -lgsl -lgslcblas
SOURCES = LikelihoodType.cpp Model.cpp RandomNumberGenerator.cpp Utils.cpp
OBJECTS = LikelihoodType.o Model.o RandomNumberGenerator.o Utils.o

default:
	# Compile all library components
	g++ $(CFLAGS) -c $(SOURCES)

	# Make static library
	ar rcs libdnest3.a $(OBJECTS)

	# Build executable example
	g++ $(CFLAGS) -o main main.cpp Models/SpikeSlab/SpikeSlab.cpp $(LIBS)

clean:
	rm -f *.o libdnest3.a main

