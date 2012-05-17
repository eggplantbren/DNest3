#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <algorithm>

using namespace std;

namespace DNest3
{

template<class ModelType>
Sampler<ModelType>::Sampler(const Options& options)
:options(options)
,particles(options.numParticles)
,logL(options.numParticles)
,indices(options.numParticles, 0)
,levels(1, Level(0., -1E300, 0.))
,logLKeep(0)
,initialised(false)
,count(0)
{

}

template<class ModelType>
void Sampler<ModelType>::initialise()
{
	assert(!initialised);
	cout<<"# Generating "<<options.numParticles<<
			" particles from the prior..."<<flush;
	for(int i=0; i<options.numParticles; i++)
	{
		particles[i].fromPrior();
		logL[i] = LikelihoodType
				(particles[i].logLikelihood(), randomU());
		logLKeep.push_back(logL[i]);
	}
	initialised = true;
	cout<<"done."<<endl;
}

template<class ModelType>
void Sampler<ModelType>::run()
{
	if(!initialised)
		initialise();

	while(true)
		step();
}

template<class ModelType>
void Sampler<ModelType>::step()
{
	// Move a particle
	int which = randInt(options.numParticles);
	if(randomU() <= 0.5)
	{
		updateParticle(which);
		updateIndex(which);
	}
	else
	{
		updateIndex(which);
		updateParticle(which);
	}
	count++;

	// Accumulate likelihoods for making a new level
	if(static_cast<int>(levels.size()) < options.maxNumLevels &&
			levels.back().get_cutoff() < logL[which])
		logLKeep.push_back(logL[which]);

	// Actually create a new level
	if(static_cast<int>(logLKeep.size()) >= options.newLevelInterval)
	{
		sort(logLKeep.begin(), logLKeep.end());
		// NOT IMPLEMENTED YET
		logLKeep.clear();
	}

	if(count%options.saveInterval == 0)
	{
		int N = count/options.saveInterval;
		cout<<"# Saving a particle to disk. N = "<<N<<"."<<endl;

		// Save the particle to file
		fstream fout;
		if(N == 0)
		{
			fout.open(options.sampleFile.c_str(), ios::out);
			fout<<"# Samples file. One sample per line."<<endl;
		}
		else
			fout.open(options.sampleFile.c_str(), ios::out|ios::app);
		particles[which].print(fout); fout<<endl;
		fout.close();

		// Save the particle's info
		if(N == 0)
		{
			fout.open(options.sampleInfoFile.c_str(), ios::out);
			fout<<"# index, logLikelihood, tieBreaker, ID."<<endl;
		}
		else
			fout.open(options.sampleInfoFile.c_str(), ios::out|ios::app);
		fout<<indices[which]<<' '<<logL[which].logL<<' '
				<<logL[which].tieBreaker<<' '<<which<<endl;
		fout.close();
	}
}

template<class ModelType>
void Sampler<ModelType>::updateParticle(int which)
{
	// Copy the particle
	ModelType proposal = particles[which];
	LikelihoodType logL_proposal = logL[which];

	// Perturb the proposal particle
	double logH = proposal.perturb();
	logL_proposal.tieBreaker += pow(10., 1.5 - 6.*randomU())*randn();
	logL_proposal.tieBreaker = mod(logL_proposal.tieBreaker, 1.);
	if(logH > 0.)
		logH = 0.;

	if(levels[indices[which]].get_cutoff() < logL_proposal
		&& randomU() <= exp(logH))
	{
		// Accept
		particles[which] = proposal;
		logL[which] = logL_proposal;
	}
}

template<class ModelType>
void Sampler<ModelType>::updateIndex(int which)
{
	cout<<which;
}

} // namespace DNest3

