# Set compilation environment
env = Environment(CC='g++')

# Optimisation arguments to g++
opt = env.Clone(CCFLAGS = '-O3 -DNDEBUG -fPIC -Wall -Wextra -ansi -pedantic')

# Alternative: 'debug' arguments
debug = env.Clone(CCFLAGS = '-O0 -g -fPIC -Wall -Wextra -ansi -pedantic')

# Choose one of the above environments
my_env = opt

# Compile .cpp files to .o
my_env.Object(Glob('*.cpp'))

# Make static library file
my_env.StaticLibrary('dnest3', Glob('*.o'))

# List of examples to be compiled
examples = ['SpikeSlab']

# SConscript files for examples
examples = ['Examples/' + e + '/SConscript' for e in examples]

# Build the examples
SConscript(examples, exports=['my_env'])

