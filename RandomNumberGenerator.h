#ifndef _RandomNumberGenerator_
#define _RandomNumberGenerator_

#include <gsl/gsl_rng.h>
#include <vector>

namespace NewSampler
{

/*
* RandomNumberGenerator is a wrapper around GSL's random number generator.
* This class can be instantiated like a regular class, but throughout DNest
* it is used in a different way. There are thread-specific instances - i.e.
* each thread has one and just one RandomNumberGenerator, and these can be
* accessed by calling randomU(), randInt(int), or randn() as though they
* were global functions.
*/

class RandomNumberGenerator
{
	// Define a single RandomNumberGenerator
	private:
		gsl_rng* rng;

	public:
		RandomNumberGenerator(int seed);
		~RandomNumberGenerator();

		double randomU() const;
		double randn() const;
		int randInt(int numPossibilities) const;

		// Getter - allows access to gsl object
		// for calls to other distributions
		gsl_rng* get_rng() { return rng; }

};

} // namespace NewSampler

#endif

