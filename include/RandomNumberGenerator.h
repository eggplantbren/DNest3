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

#ifndef DNest3_RandomNumberGenerator_h
#define DNest3_RandomNumberGenerator_h

#include <gsl/gsl_rng.h>
#include <vector>
#include <boost/thread/tss.hpp>

namespace DNest3
{

/*
* RandomNumberGenerator is a wrapper around GSL's random number generator.
* It supports individual objects as well as a global instance.
*/

class RandomNumberGenerator
{
	private:
		gsl_rng* rng;
		int seed;

	public:
		// Seed with -1
		RandomNumberGenerator();

		// Use given seed
		RandomNumberGenerator(int seed);

		// Free the memory
		~RandomNumberGenerator();

		// Set seed after initialisation
		void set_seed(int seed);

		// Getter
		int get_seed() const { return seed; }

		// U(0, 1)
		double randomU() const;

		// N(0, 1)
		double randn() const;

		// U({0, 1, ..., numPossibilities-1)
		int randInt(int numPossibilities) const;

		// Getter - allows access to gsl object
		// for calls to other distributions
		gsl_rng* get_rng() { return rng; }

	// Static stuff for a global instance
	private:
		static boost::thread_specific_ptr<RandomNumberGenerator> instance;

	public:
		// Get instance
		static RandomNumberGenerator& get_instance()
		{ return *(instance.get()); }

		// Initialise instance
		static void initialise_instance();
};

// Global RNG functions using RandomNumberGenerator::instance
double randomU();
double randn();
int randInt(int numPossibilities);

} // namespace DNest3

#endif

