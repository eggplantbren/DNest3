#ifndef _Sampler_
#define _Sampler_

#include <vector>
#include "LikelihoodType.h"

namespace DNest3
{

template<class ModelType>
class Sampler
{
	private:
		// Options options;
		std::vector<ModelType> walkers;
		std::vector<LikelihoodType> logL;

	public:
		//Sampler(Options& options);

};

}

#include "SamplerImpl.h"

#endif

