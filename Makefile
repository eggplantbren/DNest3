# Compiler settings
CPP = g++
# CFLAGS = -O0 -g -Wall -Wextra -ansi -pedantic  # Development
CFLAGS = -O2 -Wall -Wextra -ansi -pedantic -DNDEBUG  # Optimized

# Directory structure
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = lib

# Filenames
FILENAMES = CommandLineOptions Level LikelihoodType Model Options RandomNumberGenerator Utils
SOURCES = $(foreach f, $(FILENAMES), $(SRC_DIR)/$(f).cpp)
OBJECTS = $(foreach f, $(FILENAMES), $(SRC_DIR)/$(f).o)
LIB_NAME = $(BUILD_DIR)/libdnest3.a

# Examples
EXAMPLE_DIR = Examples
EXAMPLES = SpikeSlab FitSine

# The default build rule
.cpp.o:
	$(CPP) $(CFLAGS) -I$(INCLUDE_DIR) -o $*.o -c $*.cpp

default: libdnest3 examples

# Build the library
libdnest3: $(OBJECTS)
	mkdir -p $(BUILD_DIR)
	ar rcs $(LIB_NAME) $(OBJECTS)

# Build the examples
examples: force_look
	$(foreach e, $(EXAMPLES), (echo "Building $(e):"; cd $(EXAMPLE_DIR)/$(e); $(MAKE) $(MFLAGS));)

clean:
	rm -f $(OBJECTS) $(LIB_NAME) *.pyc
	$(foreach e, $(EXAMPLES), (cd $(EXAMPLE_DIR)/$(e); $(MAKE) clean);)

force_look:
	true

