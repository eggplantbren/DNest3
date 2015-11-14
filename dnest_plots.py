# Copyright (c) 2009-2015 Brendon J. Brewer.
#
# This file is part of DNest3.
#
# DNest3 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# DNest3 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with DNest3. If not, see <http://www.gnu.org/licenses/>.

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

	for i in range(0, ID.max() + 1):
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

