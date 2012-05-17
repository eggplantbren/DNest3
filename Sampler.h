#ifndef _Sampler_
#define _Sampler_

#include <vector>
#include "LikelihoodType.h"
#include "Options.h"
#include "Level.h"

namespace DNest3
{

template<class ModelType>
class Sampler
{
	private:
		// Options (most useful comment ever)
		Options options;

		// Stuff pertaining to the particles
		std::vector<ModelType> particles;
		std::vector<LikelihoodType> logL;
		std::vector<int> indices;

		// Stuff pertaining to the level structure
		std::vector<Level> levels;
		std::vector<LikelihoodType> logLKeep;

		bool initialised;

	public:
		// Constructor: Pass in Options object
		Sampler(const Options& options);

		// Do the sampling!
		void run();

	private:
		// These are helper methods -- not part of the public interface

		// Initialise all objects from the prior
		void initialise();

		// Choose a particle and do one step with it
		void step();

		// Do a M-H step of a particle
		void updateParticle(int which);

		// Do a M-H step of an index
		void updateIndex(int which);
};

}

#include "SamplerImpl.h"

#endif

