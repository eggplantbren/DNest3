from pylab import *

img = exp(loadtxt('shake.txt'))
sample = loadtxt('sample.txt')
weights = loadtxt('weights.txt')
weights = weights/max(weights)
print(sample.shape, weights.shape)

plt.ion()

for i in xrange(0, sample.shape[0]):
	samp = sample[0:(i+1), :]
	wht  = weights[0:(i+1)]

	if samp.shape[0] > 15:
		samp = samp[-15:, :]
		wht  = wht[-15:]

	plt.hold(False)
	imshow(img, extent=[0, 1, 0, 1], cmap='gray')
	xlabel('$x$', fontsize=16)
	ylabel('$y$', fontsize=16)

	plt.hold(True)
	scatter(samp[:,0], samp[:,1], color='y', marker='o', s=500*wht, linewidths=1)
	plot(samp[:,0], samp[:,1], 'r')

	axis('scaled')
	xlim([0, 1])
	ylim([0, 1])

	draw()

ioff()
show()

