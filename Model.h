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

#ifndef _Model_
#define _Model_

#include <vector>
#include <ostream>

namespace DNest3
{

/* Abstract base class for models */
class Model
{
	protected:
		// Parameters go here

	public:
		// Generate the point from the prior
		virtual void fromPrior() = 0;

		// Metropolis-Hastings proposals
		virtual double perturb() = 0;

		// Likelihood function
		virtual double logLikelihood() const = 0;

		// Print to stream
		virtual void print(std::ostream& out) const = 0;
};

} // namespace DNest3

#endif

