#ifndef _Options_
#define _Options_

/*
* Options for the Sampler
*/

namespace DNest3
{

class Options
{
	public:
		unsigned int numParticles;
		unsigned int newLevelInterval;
		unsigned int saveInterval;
		unsigned int maxNumLevels;
		double lambda, beta;
		unsigned int maxNumSamples;

	public:
		Options(unsigned int numParticles,
			unsigned int newLevelInterval,
			unsigned int saveInterval,
			unsigned int maxNumLevels,
			double lambda,
			double beta,
			unsigned int maxNumSamples);

		Options(const char* filename);
		void load(const char* filename);
};

} // namespace DNest3

#endif

