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
		std::vector<LikelihoodType> logLKeep;

	public:
		Sampler(const Options& options);

};

}

#include "SamplerImpl.h"

#endif

