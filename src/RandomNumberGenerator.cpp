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
RandomNumberGenerator RandomNumberGenerator::instance;

RandomNumberGenerator::RandomNumberGenerator()
:rng(gsl_rng_alloc(gsl_rng_mt19937))
{
	setSeed(-1);
}

RandomNumberGenerator::RandomNumberGenerator(int seed)
:rng(gsl_rng_alloc(gsl_rng_taus))
{
	setSeed(seed);
}

RandomNumberGenerator::~RandomNumberGenerator()
{
	gsl_rng_free(rng);
}

void RandomNumberGenerator::setSeed(int seed)
{
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

double randomU()
{
	return RandomNumberGenerator::get_instance().randomU();
}

double randn()
{
	return RandomNumberGenerator::get_instance().randn();
}

int randInt(int numPossibilities)
{
	return RandomNumberGenerator::get_instance()
					.randInt(numPossibilities);
}

} // namespace DNest3

