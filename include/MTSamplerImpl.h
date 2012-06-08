#include <boost/bind.hpp>

namespace DNest3
{

template<class ModelType>
const int MTSampler<ModelType>::skip = 1000;

template<class ModelType>
MTSampler<ModelType>::MTSampler(int numThreads, const Options& options)
:bMutex(new boost::mutex)
,barrier(new boost::barrier(numThreads))
,samplers(numThreads, Sampler<ModelType>(options))
{
	levels = samplers[0].levels;
	if(numThreads > 1)
		for(size_t i=0; i<samplers.size(); i++)
			samplers[i].primary = false;
}

template<class ModelType>
MTSampler<ModelType>::~MTSampler()
{
	delete barrier;
	delete bMutex;
}

template<class ModelType>
void MTSampler<ModelType>::loadLevels(const char* filename)
{
	samplers[0].loadLevels(filename);
	for(size_t i=1; i<samplers.size(); i++)
		samplers[i].levels = samplers[0].levels;
}

template<class ModelType>
void MTSampler<ModelType>::run()
{
	unsigned long firstSeed = RandomNumberGenerator::get_instance().
					get_seed();

	boost::thread_group threads;
	for(size_t i=0; i<samplers.size(); i++)
		threads.create_thread(boost::bind
			(&MTSampler<ModelType>::runThread, this, i, firstSeed));
	threads.join_all();
}

template<class ModelType>
void MTSampler<ModelType>::runThread(int thread, unsigned long firstSeed)
{
	RandomNumberGenerator::initialise_instance();
	RandomNumberGenerator::get_instance().set_seed(firstSeed + 10*thread);

	if(samplers.size() == 1)
	{
		samplers[thread].run();
		return;
	}

	while(true)
	{
		samplers[thread].run(skip);
	}
}

} // namespace DNest3

