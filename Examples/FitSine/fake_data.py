"""
generate fake data
"""

import numpy as np
import numpy.random as rng
import matplotlib.pyplot as plt

if __name__ == '__main__':
	rng.seed(0)

	t = np.linspace(0., 100., 101)

	# Noise free data
	y = np.sin(t)

	# Errorbars
	sig = 0.3*np.ones(t.size)

	# Add noise
	y += sig*rng.randn(t.size)

	np.savetxt('fake_data.txt', np.vstack([t, y, sig]).T)
	plt.errorbar(t, y, yerr=sig, fmt='bo')
	plt.show()

