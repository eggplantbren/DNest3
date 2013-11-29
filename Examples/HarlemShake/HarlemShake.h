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

#ifndef _HarlemShake_
#define _HarlemShake_

#include "Model.h"
#include <vector>

class HarlemShake:public DNest3::Model
{
	private:
		double x, y;

	public:
		HarlemShake();
		~HarlemShake(){};
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

		// Construct letters in [0, 1]^2
		static double H(double x, double y);
		static double A(double x, double y);
		static double R(double x, double y);
		static double L(double x, double y);
		static double O(double x, double y);
		static double M(double x, double y);
		static double S(double x, double y);
		static double K(double x, double y);
		// Evaluate a gaussian at (x, y)
		static double gaussian(double x, double y, double xc, double yc, double w, double q, double theta);

		// Write out the density
		static void density();
};

#endif

