#include <boost/bind.hpp>

namespace DNest3
{

template<class ModelType>
const int MTSampler<ModelType>::skip = 1000;

template<class ModelType>
MTSampler<ModelType>::MTSampler(int numThreads, const Options& options)
:barrier(new boost::barrier(numThreads))
,samplers(numThreads, Sampler<ModelType>(options))
,numSaves(0), numStepsSinceSave(0)
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
		barrier->wait();
		if(thread == 0)
			bookKeeping();
		barrier->wait();
	}
}

template<class ModelType>
void MTSampler<ModelType>::bookKeeping()
{
	numStepsSinceSave += skip;
	combineLevels();

	// Make one giant sampler
	Sampler<ModelType> giant = samplers[0];

	for(size_t i=1; i<samplers.size(); i++)
	{
		giant.particles.insert(giant.particles.end(),
			samplers[i].particles.begin(), samplers[i].particles.end());
		giant.logLKeep.insert(giant.logLKeep.end(),
			samplers[i].logLKeep.begin(),
			samplers[i].logLKeep.end());
		samplers[i].logLKeep.clear();
	}

	if(numStepsSinceSave >= samplers[0].options.saveInterval)
	{
		// Fiddle with samplers[which] to fool it into thinking it's due to save
		long old = giant.count;
		giant.count = (numSaves+1)*samplers[0].options.saveInterval;
		giant.bookKeeping(randInt(giant.particles.size()));
		numSaves++;
		numStepsSinceSave = 0;

		// It may have added a level, grab it
		samplers[0].logLKeep = giant.logLKeep;
		levels = giant.levels;
		for(size_t i=0; i<samplers.size(); i++)
			samplers[i].levels = levels;
	}
}

template<class ModelType>
void MTSampler<ModelType>::combineLevels()
{
	std::vector<Level> old = levels;

	// Extract level stats from all samplers
	for(size_t i=0; i<levels.size(); i++)
	{
		for(size_t j=0; j<samplers.size(); j++)
		{
			levels[i].accepts += samplers[j].levels[i].accepts - old[i].accepts;
			levels[i].tries += samplers[j].levels[i].tries - old[i].tries;
			levels[i].visits += samplers[j].levels[i].visits - old[i].visits;
			levels[i].exceeds += samplers[j].levels[i].exceeds - old[i].exceeds;
		}
	}

	for(size_t i=0; i<samplers.size(); i++)
		samplers[i].levels = levels;
}

} // namespace DNest3

