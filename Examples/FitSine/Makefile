# Development
#CFLAGS = -O0 -g -Wall -Wextra -ansi -pedantic

# Optimized
CFLAGS = -O2 -Wall -Wextra -ansi -pedantic -DNDEBUG
LIBS = -ldnest3 -lgsl -lgslcblas

default:
	# Compile
	g++ $(CFLAGS) -I../.. -c main.cpp
	g++ $(CFLAGS) -I../.. -c FitSine.cpp
	g++ $(CFLAGS) -I../.. -c Data.cpp

	# Link
	g++ -L../.. -o main *.o $(LIBS)

clean:
	rm -f main *.o


