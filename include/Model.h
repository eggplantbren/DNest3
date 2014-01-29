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

#ifndef DNest3_Model_h
#define DNest3_Model_h

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
		// Destructor
		virtual ~Model();

		// Generate the point from the prior
		virtual void fromPrior() = 0;

		// Metropolis-Hastings proposals
		virtual double perturb() = 0;

		// Likelihood function
		virtual double logLikelihood() const = 0;

		// Print to stream
		virtual void print(std::ostream& out) const = 0;

		// Optional: return string with column information
		// This will become the header for sample.txt
		virtual std::string description() const;
};

} // namespace DNest3

#endif

