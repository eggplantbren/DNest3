# Development
#CFLAGS = -O0 -g -Wall -Wextra -ansi -pedantic

# Optimized
CFLAGS = -O2 -Wall -Wextra -ansi -pedantic -DNDEBUG

LIBS = -ldnest3 -lgsl -lgslcblas
SOURCES = Level.cpp LikelihoodType.cpp Model.cpp Options.cpp RandomNumberGenerator.cpp Utils.cpp
OBJECTS = Level.o LikelihoodType.o Model.o Options.o RandomNumberGenerator.o Utils.o

default:
	# Compile all library components
	g++ $(CFLAGS) -c $(SOURCES)

	# Make static library
	ar rcs libdnest3.a $(OBJECTS)

	# Build executable examples
	cd Examples/SpikeSlab; $(MAKE)

clean:
	rm -f *.o libdnest3.a main
	cd Examples/SpikeSlab; $(MAKE) clean;

