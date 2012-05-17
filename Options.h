#ifndef _Options_
#define _Options_

#include <string>

/*
* Options for the Sampler
*/

namespace DNest3
{

class Options
{
	// All Sampler specialisations can access Options members
	template<class ModelType>
	friend class Sampler;

	public:
		// Numerical options
		int numParticles;
		int newLevelInterval;
		int saveInterval;
		int maxNumLevels;
		double lambda, beta;
		int maxNumSamples;

		// Filenames
		std::string sampleFile;
		std::string sampleInfoFile;
		std::string levelsFile;

	public:
		Options(int numParticles,
			int newLevelInterval,
			int saveInterval,
			int maxNumLevels,
			double lambda,
			double beta,
			int maxNumSamples);

		Options(const char* filename);
		void load(const char* filename);
};

} // namespace DNest3

#endif

