#ifndef _RandomNumberGenerator_
#define _RandomNumberGenerator_

#include <gsl/gsl_rng.h>
#include <vector>

namespace DNest3
{

/*
* RandomNumberGenerator is a wrapper around GSL's random number generator.
*/

class RandomNumberGenerator
{
	private:
		gsl_rng* rng;
		bool initialised;

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

} // namespace DNest3

#endif

