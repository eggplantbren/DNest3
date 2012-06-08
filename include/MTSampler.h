#ifndef DNest3_MTSampler
#define DNest3_MTSampler

#include "Sampler.h"
#include <vector>

namespace DNest3
{

template<class ModelType>
class MTSampler
{
	private:
		static const int skip;

		std::vector< Sampler<ModelType> > samplers;
		std::vector<Level> levels; // levels backup

	public:
		MTSampler(int numThreads, const Options& options);

		void run(int thread, unsigned long firstSeed);

};

} // namespace DNest3

#include "MTSamplerImpl.h"

#endif

