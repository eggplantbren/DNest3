# Development
CFLAGS = -O0 -g -Wall -Wextra -ansi -pedantic

# Optimized
#CFLAGS = -O2 -Wall -Wextra -ansi -pedantic -DNDEBUG

default:
	# Compile all library components
	g++ $(CFLAGS) -c *.cpp

	# Make static library
	ar rcs libdnest3.a *.o

	# Compile all examples
	g++ $(CFLAGS) -c Models/SpikeSlab/*.cpp

