#include "RandomNumberGenerator.h"
#include <gsl/gsl_randist.h>
#include <cmath>

using namespace std;

namespace NewSampler
{
RandomNumberGenerator::RandomNumberGenerator(int seed)
{
	rng = gsl_rng_alloc(gsl_rng_taus);
	gsl_rng_set(rng, seed);
}

RandomNumberGenerator::~RandomNumberGenerator()
{
	gsl_rng_free(rng);
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

} // namespace NewSampler

