"""
This script loads the output from the run and animates the exploration
on a convergence plot.
"""

import numpy as np
import matplotlib.pyplot as plt

def transform(x):
	return np.log(x/1.989E30)

# The parameter
parameter, walker = 0, 0

sample = np.atleast_2d(np.loadtxt('sample.txt'))
sample_info = np.atleast_2d(np.loadtxt('sample_info.txt'))

# Optionally, transform the parameter
#sample[:,parameter] = transform(sample[:,parameter])

plt.ion()
plt.hold(False)

# Single out a particular walker
x = sample_info[sample_info[:,3] == walker, 0]
y = sample[sample_info[:,3] == walker, parameter]

for i in xrange(0, len(x)):
	# Plot the regular convergence plot "in the background"
	plt.hold(False)
	plt.plot(sample_info[:,0], sample[:,parameter], 'b.', markersize=1)
	plt.xlabel('Level')
	plt.ylabel('Parameter')

	# Don't go all the way into the past
	xx = x[0:(i+1)]
	yy = y[0:(i+1)]

	if len(xx) > 5:
		xx = xx[-5:]
		yy = yy[-5:]

	plt.hold(True)
	plt.plot(xx, yy, 'ro-', markersize=10)
	plt.title('{a} / {b}'.format(a=(i+1), b=len(x)))
	plt.draw()

plt.ioff()
plt.show()

