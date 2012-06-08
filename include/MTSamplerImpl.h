#include <boost/thread.hpp>
#include <boost/bind.hpp>

namespace DNest3
{

template<class ModelType>
const int MTSampler<ModelType>::skip = 1000;

template<class ModelType>
MTSampler<ModelType>::MTSampler(int numThreads, const Options& options)
:samplers(numThreads, Sampler<ModelType>(options))
{
	levels = samplers[0].levels;
}

template<class ModelType>
void MTSampler<ModelType>::run(int thread, unsigned long firstSeed)
{
	RandomNumberGenerator::initialise_instance();
	RandomNumberGenerator::get_instance().set_seed(firstSeed + 10*thread);
}

} // namespace DNest3

