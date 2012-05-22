/*
* Copyright (c) 2009, 2010, 2011, 2012 Brendon J. Brewer.
*
* This file is part of DNest3.
*
* DNest3 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DNest3 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DNest3. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _FitSine_
#define _FitSine_

#include "Model.h"
#include <vector>

class FitSine:public DNest3::Model
{
	private:
		// The highest number of frequencies possible
		static const int maxNumComponents;

		// Number of frequencies present
		int numComponents;

		// Amplitudes hyperparameter and its limits
		double muAmplitudes, minLogMu, maxLogMu, rangeLogMu;

		// Frequency limits
		double minLogFreq, maxLogFreq, rangeLogFreq;

		// Amplitudes
		std::vector<double> amplitudes;

		// Actual frequencies
		std::vector<double> frequencies;

		// Actual phases
		std::vector<double> phases;

		// Simulated data for comparison with actual data
		std::vector<double> mockData;

		// How many times since mock data was computed from scratch
		int staleness;

		// Calculate mock data from scratch
		void calculateMockData();

		// Add one frequency to mock data
		void addComponent(double amplitude, double frequency,
					double phase);

		// Perturbers
		double perturb1();
		double perturb2();
		double perturb3();

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

		// Return string with column information
		std::string description() const;

};

#endif

