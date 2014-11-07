# Display file for Gravity
from pylab import *

output = atleast_2d(loadtxt('sample.txt'))

figure(figsize=(8, 7))
ion()
hold(False)
for i in xrange(0, output.shape[0]):
	model = output[i, :]
	x, y = model[0:200], model[200:400]
	plot(x, y, 'k.')
	axis([0, 1, 0, 1])
	title(i+1)
	draw()

ioff()
show()

