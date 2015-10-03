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

#include "RandomNumberGenerator.h"
#include <gsl/gsl_randist.h>
#include <cmath>
#include <cassert>

using namespace std;

namespace DNest3
{

// Redeclare the static instance
#ifndef DNest3_No_Boost
boost::thread_specific_ptr<RandomNumberGenerator> RandomNumberGenerator::instance;
#else
RandomNumberGenerator RandomNumberGenerator::instance;
#endif

RandomNumberGenerator::RandomNumberGenerator()
:rng(gsl_rng_alloc(gsl_rng_mt19937))
{
	set_seed(0);
}

RandomNumberGenerator::RandomNumberGenerator(unsigned long seed)
:rng(gsl_rng_alloc(gsl_rng_taus))
{
	set_seed(seed);
}

RandomNumberGenerator::~RandomNumberGenerator()
{
	gsl_rng_free(rng);
}

void RandomNumberGenerator::set_seed(int seed)
{
	this->seed = seed;
	gsl_rng_set(rng, seed);
}

double RandomNumberGenerator::randomU() const
{
	return gsl_rng_uniform_pos(rng);
}

double RandomNumberGenerator::randn() const
{
	return gsl_ran_gaussian(rng, 1.0);
}

int RandomNumberGenerator::randInt(int numPossibilities) const
{
	return (int)floor(numPossibilities*this->randomU());
}

void RandomNumberGenerator::initialise_instance()
{
	#ifndef DNest3_No_Boost
	instance.reset(new RandomNumberGenerator);
	#endif
}

double randomU()
{
	return RandomNumberGenerator::get_instance().randomU();
}

double randn()
{
	return RandomNumberGenerator::get_instance().randn();
}

double randh()
{
	return pow(10., 1.5 - 6.*randomU())*randn();
}

int randInt(int numPossibilities)
{
	return RandomNumberGenerator::get_instance()
					.randInt(numPossibilities);
}

} // namespace DNest3

