#ifndef DNest3_MTSampler
#define DNest3_MTSampler

#include "Sampler.h"
#include <vector>
#include <boost/thread.hpp>

namespace DNest3
{

template<class ModelType>
class MTSampler
{
	private:
		static const int skip;

		boost::barrier* barrier;

		std::vector< Sampler<ModelType> > samplers;
		std::vector<Level> levels; // levels backup

	public:
		// Constructor: Pass in Options object
		MTSampler(int numThreads, const Options& options);

		// Destructor - free heap memory
		~MTSampler();

		// Load levels from file
		void loadLevels(const char* filename);

		// Do the sampling!
		void run();

	private:
		// These are helper methods -- not part of the public interface
		void runThread(int thread, unsigned long firstSeed);

		void bookKeeping();

		unsigned long logLKeep_size() const;


};

} // namespace DNest3

#include "MTSamplerImpl.h"

#endif

