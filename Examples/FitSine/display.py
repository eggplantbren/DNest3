"""
Load models and display them
"""

import numpy as np
import matplotlib.pyplot as plt

maxNumComponents = 100
data = np.atleast_2d(np.loadtxt('fake_data.txt'))
sample = np.atleast_2d(np.loadtxt('posterior_sample.txt'))

start = 0

onFraction = sample[:, start]
start += 1

muAmplitudes = sample[:, start]
start += 1

amplitudes = sample[:, start:start+maxNumComponents]
start += maxNumComponents

frequencies = sample[:, start:start+maxNumComponents]
start += maxNumComponents

phases = sample[:, start:start+maxNumComponents]
start += maxNumComponents

plt.ion()
for i in xrange(0, sample.shape[0]):
	
	mockData = np.zeros(data.shape[0])
	for j in xrange(0, maxNumComponents):
		mockData += amplitudes[i, j]*np.sin(2*np.pi*frequencies[i, j]*data[:,0] + phases[i, j])

	plt.hold(False)
	plt.errorbar(data[:,0], data[:,1], yerr=data[:,2], fmt='bo')
	plt.hold(True)
	plt.plot(data[:,0], mockData, 'r')
	plt.axis([-1., 101., -5., 5.])
	plt.xlabel('Time')
	plt.ylabel('y')
	plt.title(i+1)
	plt.draw()

plt.ioff()
plt.show()

