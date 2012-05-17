#ifndef _NewSampler_
#define _NewSampler_

#include <vector>
#include "LikelihoodType.h"

namespace NewSampler
{

template<class ModelType>
class NewSampler
{
	private:
		// Options options;
		std::vector<ModelType> walkers;
		std::vector<LikelihoodType> logL;

	public:
		//NewSampler(Options& options);

};

}

#include "NewSamplerImpl.h"

#endif

