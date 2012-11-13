# Compiler settings
CPP = g++
# Optimized
CFLAGS = -O2 -Wall -Wextra -ansi -pedantic -DNDEBUG

# Directory structure
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = lib

# Filenames
FILENAMES = CommandLineOptions Level LikelihoodType Model Options RandomNumberGenerator Utils
SOURCES = $(foreach f, $(FILENAMES), $(SRC_DIR)/$(f).cpp)
OBJECTS = $(foreach f, $(FILENAMES), $(SRC_DIR)/$(f).o)
LIB_NAME = libdnest3

# Examples
EXAMPLE_DIR = Examples
EXAMPLES = SpikeSlab FitSine

# The default build rule
.cpp.o:
	$(CPP) $(CFLAGS) -I$(INCLUDE_DIR) -o $*.o -c $*.cpp

default: examples

# Build the library
libdnest3: $(OBJECTS)
	mkdir -p $(BUILD_DIR)
	ar rcs $(BUILD_DIR)/$(LIB_NAME).a $(OBJECTS)

shared: $(OBJECTS)
	mkdir -p $(BUILD_DIR)
	$(CPP) -shared $(CLIBS) -o $(BUILD_DIR)/$(LIB_NAME).so $(OBJECTS)

# Build the examples
examples: shared force_look
	$(foreach e, $(EXAMPLES), (echo "Building $(e):"; cd $(EXAMPLE_DIR)/$(e); $(MAKE) $(MFLAGS));)

clean:
	rm -f $(OBJECTS) $(LIB_NAME) *.pyc $(INCLUDE_DIR)/*.gch
	$(foreach e, $(EXAMPLES), (cd $(EXAMPLE_DIR)/$(e); $(MAKE) clean);)

force_look:
	true

