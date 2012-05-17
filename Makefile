CFLAGS = -O0 -g -Wall -Wextra -ansi -pedantic # Development
# CFLAGS = -O2 -Wall -Wextra -ansi -pedantic -DNDEBUG # Optimized

default: libnewsampler tests

libnewsampler: RandomNumberGenerator.o
	ar rcs libnewsampler.a RandomNumberGenerator.o

RandomNumberGenerator.o: RandomNumberGenerator.cpp RandomNumberGenerator.h
	g++ $(CFLAGS) -c RandomNumberGenerator.cpp

tests:


clean:
	rm -f *.o libnewsampler.a

