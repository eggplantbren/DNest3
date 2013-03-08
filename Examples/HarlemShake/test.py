from pylab import *
from os import *
from postprocess import *

n = 100
results = empty(n)

for i in xrange(0, n):
	system('./main')
	results[i] = postprocess(plot=False)[0]
	savetxt('results.txt', results[0:(i+1)])

# Results from standard Metropolis: 0.357. Based on n=55
