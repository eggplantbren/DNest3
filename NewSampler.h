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
		ModelType model;
		std::vector< std::vector<double> > walkers;
		std::vector<double> logP;
		std::vector<LikelihoodType> logL;

	public:
		//NewSampler(Options& options);

};

}

#include "NewSamplerImpl.h"

#endif

