import numpy as np
import matplotlib.pyplot as plt

def diffusion_plot():
	"""
	Plot a nice per-particle diffusion plot.
	"""

	sample_info = np.atleast_2d(np.loadtxt('sample_info.txt'))
	ID = sample_info[:,3].astype('int')
	j = sample_info[:,0].astype('int')

	ii = np.arange(1, sample_info.shape[0] + 1)

	for i in xrange(0, ID.max() + 1):
		which = np.nonzero(ID == i)[0]
		plt.plot(ii[which], j[which])

	plt.xlabel('Iteration')
	plt.ylabel('Level')
	plt.show()

def levels_plot():
	"""
	Plot the differences between the logl values of the levels.
	"""
	levels = np.loadtxt('levels.txt')

	plt.plot(np.log10(np.diff(levels[:,1])))
	plt.ylim([-1, 4])
	plt.axhline(0., color='r')
	plt.axhline(np.log10(np.log(10.)), color='g')
	plt.xlabel('Level')
	plt.ylabel('Delta log likelihood')
	plt.show()

