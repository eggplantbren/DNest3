#include "RandomNumberGenerator.h"
#include <iostream>
#include <cassert>

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
}

template<class ModelType>
void Sampler<ModelType>::updateParticle(int which)
{
	cout<<which;
}

template<class ModelType>
void Sampler<ModelType>::updateIndex(int which)
{
	cout<<which;
}

} // namespace DNest3

