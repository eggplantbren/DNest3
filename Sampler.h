#ifndef _Sampler_
#define _Sampler_

#include <vector>
#include "LikelihoodType.h"
#include "Options.h"

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
	//	std::vector<Level> levels;
		std::vector<LikelihoodType> logLKeep;

		bool initialised;

	public:
		// Constructor: Pass in Options object
		Sampler(const Options& options);

		// Initialise all objects from the prior
		void initialise();

		// Do the sampling!
		void run();

		// Do one step
		void step();
};

}

#include "SamplerImpl.h"

#endif

