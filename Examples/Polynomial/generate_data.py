"""
Generate simulated data from a 4th-order polynomial
so that we can fit it in DNest.
"""

import numpy as np
import numpy.random as rng
import matplotlib.pyplot as plt

# For reproducibility
rng.seed(0)

# True coefficients
a = np.array([0.3, -0.7, 1.7, 1.3, -0.1])

# Noise level
sig = 0.2

# Number of data points
num = 101

# x values of data points
x = np.linspace(-1., 1., num)

# y values of data points -- create the curve
y = np.zeros(num)
for i in xrange(0, a.size):
	y += a[i]*x**i

# Add noise
y += sig*rng.randn(num)

# Arrange the data into a (num x 3) array (x, y, sig)
data = np.empty((num, 3))
data[:,0], data[:,1], data[:,2] = x, y, sig*np.ones(num)

# Save the data to a file
np.savetxt('data.txt', data)

# Plot the data
plt.errorbar(data[:,0], data[:,1], yerr=data[:,2], fmt='b.')
plt.show()

