#ifndef _RandomNumberGenerator_
#define _RandomNumberGenerator_

#include <gsl/gsl_rng.h>
#include <vector>

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

	public:
		// Seed with -1
		RandomNumberGenerator();

		// Use given seed
		RandomNumberGenerator(int seed);

		// Free the memory
		~RandomNumberGenerator();

		// Set seed after initialisation
		void setSeed(int seed);

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
		static RandomNumberGenerator instance;

	public:
		// Get instance
		static RandomNumberGenerator& get_instance()
		{ return instance; }
};

// Global RNG functions using RandomNumberGenerator::instance
double randomU();
double randn();
int randInt(int numPossibilities);

} // namespace DNest3

#endif

