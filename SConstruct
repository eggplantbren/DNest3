# Set compilation environment
env = Environment(CC='g++')

# Stuff to check for existence of header files and libraries
conf = Configure(env)
if not conf.CheckCHeader('gsl/gsl_rng.h'):
	print('Did not find gsl_rng.h. Check your GSL installation.')
if not conf.CheckCXXHeader('boost/thread.hpp'):
	print('Did not find thread.hpp. Check your Boost thread installation.')

env = conf.Finish()

# Optimisation arguments to g++
opt = env.Clone(CCFLAGS = '-O3 -DNDEBUG -Wall -Wextra -ansi -pedantic')

# Alternative: 'debug' arguments
debug = env.Clone(CCFLAGS = '-O0 -g -Wall -Wextra -ansi -pedantic')

# Choose one of the above environments
my_env = opt

# Compile .cpp files to .o
my_env.Object(Glob('*.cpp'))

# Make static library file
my_env.StaticLibrary('dnest3', Glob('*.o'))

# List of examples to be compiled
examples = ['ABC', 'Atoms', 'FitSine', 'HarlemShake', 'SpikeSlab', 'Template']

# SConscript files for examples
examples = ['Examples/' + e + '/SConscript' for e in examples]

# Build the examples
SConscript(examples, exports=['my_env'])

