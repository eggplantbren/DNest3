# Development
CFLAGS = -O0 -g -Wall -Wextra -ansi -pedantic

# Optimized
#CFLAGS = -O2 -Wall -Wextra -ansi -pedantic -DNDEBUG

default: libnewsampler.a main

### Object files
main.o: main.cpp Sampler.h SamplerImpl.h LikelihoodType.h Models/SpikeSlab/SpikeSlab.h
	g++ $(CFLAGS) -c main.cpp

LikelihoodType.o: LikelihoodType.cpp LikelihoodType.h
	g++ $(CFLAGS) -c LikelihoodType.cpp

Model.o: Model.cpp Model.h
	g++ $(CFLAGS) -c Model.cpp

RandomNumberGenerator.o: RandomNumberGenerator.cpp RandomNumberGenerator.h
	g++ $(CFLAGS) -c RandomNumberGenerator.cpp

SpikeSlab.o: Models/SpikeSlab/SpikeSlab.cpp Models/SpikeSlab/SpikeSlab.h Model.h
	g++ $(CFLAGS) -c Models/SpikeSlab/SpikeSlab.cpp

Utils.o: Utils.cpp Utils.h
	g++ $(CFLAGS) -c Utils.cpp

### Static library
libnewsampler.a: LikelihoodType.o Model.o RandomNumberGenerator.o Utils.o
	ar rcs libnewsampler.a LikelihoodType.o Model.o RandomNumberGenerator.o Utils.o

### Main executable
main: main.o SpikeSlab.o libnewsampler.a
	g++ -o main main.o SpikeSlab.o -lnewsampler -lgsl -lgslcblas

clean:
	rm -f *.o libnewsampler.a

