/*
* Copyright (c) 2009, 2010, 2011, 2012 Brendon J. Brewer.
*
* This file is part of DNest.
*
* DNest is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DNest is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DNest. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _FitSine_
#define _FitSine_

#include "Model.h"
#include <vector>

class FitSine:public DNest3::Model
{
	private:
		// The highest number of frequencies possible
		static const int maxNumFrequencies;

		// Latent uniforms for amplitudes
		std::vector<double> u_amplitudes;

		// Amplitudes hyperparameter and its limits
		double muAmplitudes, minLogMu, maxLogMu, rangeLogMu;

		// Frequency limits
		double minLogFreq, maxLogFreq, rangeLogFreq;

		// Actual frequencies and phases
		std::vector<double> frequencies;
		std::vector<double> phases;

		// Transform latent u_amplitude into an amplitude
		double transform(double u_amplitude) const;

	public:
		FitSine();

		// Generate the point from the prior
		void fromPrior();

		// Metropolis-Hastings proposals
		double perturb();

		// Likelihood function
		double logLikelihood() const;

		// Print to stream
		void print(std::ostream& out) const;


};

#endif

