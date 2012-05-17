#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>

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

	if(count%options.saveInterval == 0)
	{
		// Save a particle to file
		int N = count/options.saveInterval;
		fstream fout;
		if(N == 0)
			fout.open(options.sampleFile.c_str(), ios::out);
		else
			fout.open(options.sampleFile.c_str(), ios::out|ios::app);
		particles[which].print(fout); fout<<endl;
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

