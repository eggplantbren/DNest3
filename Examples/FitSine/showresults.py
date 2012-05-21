import copy
import numpy as np
import matplotlib.pyplot as plt

def logsumexp(values):
	biggest = np.max(values)
	x = values - biggest
	result = np.log(np.sum(np.exp(x))) + biggest
	return result

def logdiffexp(x1, x2):
	biggest = x1
	xx1 = x1 - biggest
	xx2 = x2 - biggest
	result = np.log(np.exp(xx1) - np.exp(xx2)) + biggest
	return result


numResampleLogX = 1
temperature = 1.0

levels = np.atleast_2d(np.loadtxt("levels.txt"))
sample_info = np.atleast_2d(np.loadtxt("sample_info.txt"))
sample = np.atleast_2d(np.loadtxt("sample.txt"))

if sample.shape[0] != sample_info.shape[0]:
	print('ERROR. Size mismatch. The usual cause is that you\'re still running DNest and it wrote to the files that showresults loads. Try suspending DNest temporarily.')
	exit()

if numResampleLogX > 1:
	plt.ion()

plt.figure(1)
plt.plot(sample_info[:,0])
plt.xlabel("Iteration")
plt.ylabel("Level")
if numResampleLogX > 1:
	plt.draw()

plt.figure(2)
plt.subplot(2,1,1)
plt.plot(np.diff(levels[:,0]))
plt.ylabel("Compression")
plt.xlabel("Level")
xlim = plt.gca().get_xlim()
if numResampleLogX > 1:
	plt.draw()

plt.subplot(2,1,2)
good = np.nonzero(levels[:,4] > 0)[0]
plt.plot(levels[good,3]/levels[good,4])
plt.xlim(xlim)
plt.ylim([-0.1, 1.1])
plt.xlabel("Level")
plt.ylabel("MH Acceptance")
if numResampleLogX > 1:
	plt.draw()

# Convert to lists of tuples
logl_levels = [(levels[i,1], levels[i, 2]) for i in xrange(0, levels.shape[0])] # logl, tiebreaker
logl_samples = [(sample_info[i, 1], sample_info[i, 2], i) for i in xrange(0, sample.shape[0])] # logl, tiebreaker, id
logx_samples = np.zeros((sample_info.shape[0], numResampleLogX))
logp_samples = np.zeros((sample_info.shape[0], numResampleLogX))
logP_samples = np.zeros((sample_info.shape[0], numResampleLogX))
P_samples = np.zeros((sample_info.shape[0], numResampleLogX))
logz_estimates = np.zeros((numResampleLogX, 1))
H_estimates = np.zeros((numResampleLogX, 1))

# Find sandwiching level for each sample
sandwich = sample_info[:,0].copy().astype('int')
for i in xrange(0, sample.shape[0]):
	while sandwich[i] < levels.shape[0]-1 and logl_samples[i] > logl_levels[sandwich[i] + 1]:
		sandwich[i] += 1

for z in xrange(0, numResampleLogX):
	# For each level
	for i in range(0, levels.shape[0]):
		# Find the samples sandwiched by this level
		which = np.nonzero(sandwich == i)[0]
		logl_samples_thisLevel = [] # (logl, tieBreaker, ID)
		for j in xrange(0, len(which)):
			logl_samples_thisLevel.append(copy.deepcopy(logl_samples[which[j]]))
		logl_samples_thisLevel = sorted(logl_samples_thisLevel)
		N = len(logl_samples_thisLevel)

		# Generate intermediate logx values
		logx_max = levels[i, 0]
		if i == levels.shape[0]-1:
			logx_min = -1E300
		else:
			logx_min = levels[i+1, 0]
		Umin = np.exp(logx_min - logx_max)

		if N == 0 or numResampleLogX > 1:
			U = Umin + (1. - Umin)*np.random.rand(len(which))
		else:
			U = Umin + (1. - Umin)*np.linspace(1./(N+1), 1. - 1./(N+1), N)
		logx_samples_thisLevel = np.sort(logx_max + np.log(U))[::-1]

		for j in xrange(0, which.size):
			logx_samples[logl_samples_thisLevel[j][2]][z] = logx_samples_thisLevel[j]

			if j != which.size - 1:
				left = logx_samples_thisLevel[j+1]
			elif i == levels.shape[0]-1:
				left = -1E300
			else:
				left = levels[i+1][0]
				
			if j != 0:
				right = logx_samples_thisLevel[j-1]
			else:
				right = levels[i][0]

			logp_samples[logl_samples_thisLevel[j][2]][z] = np.log(0.5) + logdiffexp(right, left)

	logl = sample_info[:,1]/temperature

	logp_samples[:,z] = logp_samples[:,z] - logsumexp(logp_samples[:,z])
	logP_samples[:,z] = logp_samples[:,z] + logl
	logz_estimates[z] = logsumexp(logP_samples[:,z])
	logP_samples[:,z] -= logz_estimates[z]
	P_samples[:,z] = np.exp(logP_samples[:,z])
	H_estimates[z] = -logz_estimates[z] + np.sum(P_samples[:,z]*logl)

	plt.figure(3)
	if z == 0:
		plt.subplot(2,1,1)
		plt.plot(logx_samples[:,z], sample_info[:,1], 'b.', label='Samples')
		plt.hold(True)
		plt.plot(levels[1:,0], levels[1:,1], 'r.', label='Levels')
		plt.legend(numpoints=1, loc='lower left')
		plt.ylabel('log(L)')
		plt.title(str(z+1) + "/" + str(numResampleLogX) + ", log(Z) = " + str(logz_estimates[z][0]))
		if numResampleLogX > 1:
			plt.draw()
		xlim = plt.gca().get_xlim()

	plt.subplot(2,1,2)
	plt.hold(False)
	plt.plot(logx_samples[:,z], P_samples[:,z], 'b.')
	plt.ylabel('Posterior Weights')
	plt.xlabel('log(X)')
	plt.xlim(xlim)
	if numResampleLogX > 1:
		plt.draw()

P_samples = np.mean(P_samples, 1)
P_samples = P_samples/np.sum(P_samples)
logz_estimate = np.mean(logz_estimates)
logz_error = np.std(logz_estimates)
H_estimate = np.mean(H_estimates)
H_error = np.std(H_estimates)
ESS = np.exp(-np.sum(P_samples*np.log(P_samples+1E-300)))

print("log(Z) = " + str(logz_estimate) + " +- " + str(logz_error))
print("Information = " + str(H_estimate) + " +- " + str(H_error) + " nats.")
print("Effective sample size = " + str(ESS))

# Resample to uniform weight
N = int(ESS)
posterior_sample = np.zeros((N, sample.shape[1]))
w = P_samples
w = w/np.max(w)
np.savetxt('weights.txt', w) # Save weights
for i in xrange(0, N):
	while True:
		which = np.random.randint(sample.shape[0])
		if np.random.rand() <= w[which]:
			break
	posterior_sample[i,:] = sample[which,:]
np.savetxt("posterior_sample.txt", posterior_sample)

if numResampleLogX > 1:
	plt.ioff()
plt.show()

import display
