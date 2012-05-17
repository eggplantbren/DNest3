# Development
CFLAGS = -O0 -g -Wall -Wextra -ansi -pedantic

# Optimized
#CFLAGS = -O2 -Wall -Wextra -ansi -pedantic -DNDEBUG

default: libnewsampler.a main

### Object files
main.o: main.cpp Sampler.h SamplerImpl.h TestModel.h LikelihoodType.h
	g++ $(CFLAGS) -c main.cpp

LikelihoodType.o: LikelihoodType.cpp LikelihoodType.h
	g++ $(CFLAGS) -c LikelihoodType.cpp

Model.o: Model.cpp Model.h
	g++ $(CFLAGS) -c Model.cpp

RandomNumberGenerator.o: RandomNumberGenerator.cpp RandomNumberGenerator.h
	g++ $(CFLAGS) -c RandomNumberGenerator.cpp

TestModel.o: TestModel.cpp TestModel.h Model.h
	g++ $(CFLAGS) -c TestModel.cpp

### Static library
libnewsampler.a: LikelihoodType.o Model.o RandomNumberGenerator.o
	ar rcs libnewsampler.a LikelihoodType.o Model.o RandomNumberGenerator.o

### Main executable
main: main.o TestModel.o libnewsampler.a
	g++ -o main main.o TestModel.o -lnewsampler

clean:
	rm -f *.o libnewsampler.a

