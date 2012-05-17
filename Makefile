CFLAGS = -O0 -g -Wall -Wextra -ansi -pedantic # Development
# CFLAGS = -O2 -Wall -Wextra -ansi -pedantic -DNDEBUG # Optimized

default: libnewsampler.a

libnewsampler.a: Model.o RandomNumberGenerator.o
	ar rcs libnewsampler.a Model.o RandomNumberGenerator.o

Model.o: Model.cpp Model.h
	g++ $(CFLAGS) -c Model.cpp

RandomNumberGenerator.o: RandomNumberGenerator.cpp RandomNumberGenerator.h
	g++ $(CFLAGS) -c RandomNumberGenerator.cpp

clean:
	rm -f *.o libnewsampler.a

