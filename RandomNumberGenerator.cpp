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
:rng(gsl_rng_alloc(gsl_rng_taus))
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

