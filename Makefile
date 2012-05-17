# Development
CFLAGS = -O0 -g -Wall -Wextra -ansi -pedantic

# Optimized
#CFLAGS = -O2 -Wall -Wextra -ansi -pedantic -DNDEBUG

default: libnewsampler.a main

### Object files
main.o: main.cpp NewSampler.h
	g++ $(CFLAGS) -c main.cpp

Model.o: Model.cpp Model.h
	g++ $(CFLAGS) -c Model.cpp

RandomNumberGenerator.o: RandomNumberGenerator.cpp RandomNumberGenerator.h
	g++ $(CFLAGS) -c RandomNumberGenerator.cpp

### Static library
libnewsampler.a: Model.o RandomNumberGenerator.o
	ar rcs libnewsampler.a Model.o RandomNumberGenerator.o

### Main executable
main: main.o libnewsampler.a
	g++ -o main main.o -lnewsampler

clean:
	rm -f *.o libnewsampler.a

