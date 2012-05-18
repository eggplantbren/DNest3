# Copyright (c) 2009, 2010, 2011, 2012 Brendon J. Brewer.
#
# This file is part of DNest.
#
# DNest is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# DNest is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with DNest. If not, see <http://www.gnu.org/licenses/>.

from copy import deepcopy
from pylab import *

def logsumexp(values):
	biggest = max(values)
	x = values - biggest
	result = log(sum(exp(x))) + biggest
	return result

def logdiffexp(x1, x2):
	biggest = x1
	xx1 = x1 - biggest
	xx2 = x2 - biggest
	result = log(exp(xx1) - exp(xx2)) + biggest
	return result


numResampleLogX = 1
temperature = 1.0

levels = atleast_2d(loadtxt("levels.txt"))
sample_info = atleast_2d(loadtxt("sample_info.txt"))
sample = atleast_2d(loadtxt("sample.txt"))

if sample.shape[0] != sample_info.shape[0]:
	print('ERROR. Size mismatch. The usual cause is that you\'re still running DNest and it wrote to the files that showresults loads. Try suspending DNest temporarily.')
	exit()

ion()
figure(1)
plot(sample_info[:,0])
xlabel("Iteration")
ylabel("Level")
draw()

figure(2)
subplot(2,1,1)
plot(diff(levels[:,0]))
ylabel("Compression")
xlabel("Level")
subplot(2,1,2)
plot(levels[:,3]/levels[:,4])
ylim([0, 1])
xlabel("Level")
ylabel("MH Acceptance")
draw()

# Convert to lists of tuples
logl_levels = [(levels[i,1], levels[i, 2]) for i in range(0, levels.shape[0])] # logl, tiebreaker
logl_samples = [(sample_info[i, 1], sample_info[i, 2], i) for i in range(0, sample.shape[0])] # logl, tiebreaker, id
logx_samples = zeros((sample_info.shape[0], numResampleLogX))
logp_samples = zeros((sample_info.shape[0], numResampleLogX))
logP_samples = zeros((sample_info.shape[0], numResampleLogX))
P_samples = zeros((sample_info.shape[0], numResampleLogX))
logz_estimates = zeros((numResampleLogX, 1))
H_estimates = zeros((numResampleLogX, 1))

# Find sandwiching level for each sample
sandwich = int64(sample_info[:,0])
for i in range(0, sample.shape[0]):
	while sandwich[i] < levels.shape[0]-1 and logl_samples[i] > logl_levels[sandwich[i] + 1]:
		sandwich[i] += 1

for z in range(0, numResampleLogX):
	# For each level
	for i in range(0, levels.shape[0]):
		# Find the samples sandwiched by this level
		which = nonzero(sandwich == i)[0]
		logl_samples_thisLevel = [] # (logl, tieBreaker, ID)
		for j in range(0, len(which)):
			logl_samples_thisLevel.append(deepcopy(logl_samples[which[j]]))
		logl_samples_thisLevel = sorted(logl_samples_thisLevel)
		N = len(logl_samples_thisLevel)

		# Generate intermediate logx values
		logx_max = levels[i, 0]
		if i == levels.shape[0]-1:
			logx_min = -1E300
		else:
			logx_min = levels[i+1, 0]
		Umin = exp(logx_min - logx_max)

		if N == 0 or numResampleLogX > 1:
			U = Umin + (1.0 - Umin)*rand(len(which))
		else:
			U = Umin + (1.0 - Umin)*linspace(1.0/(N+1), 1.0 - 1.0/(N+1), N)
		logx_samples_thisLevel = sort(logx_max + log(U))[::-1]

		for j in range(0, len(which)):
			logx_samples[logl_samples_thisLevel[j][2]][z] = logx_samples_thisLevel[j]

			if j != len(which)-1:
				left = logx_samples_thisLevel[j+1]
			elif i == levels.shape[0]-1:
				left = -1E300
			else:
				left = levels[i+1][0]
				
			if j != 0:
				right = logx_samples_thisLevel[j-1]
			else:
				right = levels[i][0]

			logp_samples[logl_samples_thisLevel[j][2]][z] = log(0.5) + logdiffexp(right, left)

	logl = sample_info[:,1]/temperature

	logp_samples[:,z] = logp_samples[:,z] - logsumexp(logp_samples[:,z])
	logP_samples[:,z] = logp_samples[:,z] + logl
	logz_estimates[z] = logsumexp(logP_samples[:,z])
	logP_samples[:,z] -= logz_estimates[z]
	P_samples[:,z] = exp(logP_samples[:,z])
	H_estimates[z] = -logz_estimates[z] + sum(P_samples[:,z]*logl)

	figure(3)
	clf()
	subplot(2,1,1)
	p1 = plot(logx_samples[:,z], sample_info[:,1], 'b.', label='Samples')
	p2 = plot(levels[1:,0], levels[1:,1], 'r.', label='Levels')
	legend(numpoints=1, loc='lower left')
	ylabel('log(L)')
	title(str(z+1) + "/" + str(numResampleLogX) + ", log(Z) = " + str(logz_estimates[z][0]))
	subplot(2,1,2)
	plot(logx_samples[:,z], P_samples[:,z], 'b.')
	ylabel('Posterior Weights')
	xlabel('log(X)')
	draw()

P_samples = mean(P_samples, 1)
P_samples = P_samples/sum(P_samples)
logz_estimate = mean(logz_estimates)
logz_error = std(logz_estimates)
H_estimate = mean(H_estimates)
H_error = std(H_estimates)
ESS = exp(-sum(P_samples*log(P_samples+1E-300)))

print("log(Z) = " + str(logz_estimate) + " +- " + str(logz_error))
print("Information = " + str(H_estimate) + " +- " + str(H_error) + " nats.")
print("Effective sample size = " + str(ESS))

# Resample to uniform weight
N = int(ESS)
posterior_sample = zeros((N, shape(sample)[1]))
w = P_samples
w = w/max(w)
savetxt('weights.txt', w) # Save weights
for i in range(0, N):
	while True:
		which = randint(sample.shape[0])
		if rand() <= w[which]:
			break
	posterior_sample[i,:] = sample[which,:]
savetxt("posterior_sample.txt", posterior_sample)

ioff()
show()

import display

