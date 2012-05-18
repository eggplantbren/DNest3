#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iomanip>

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
	saveLevels();
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
	{
		bool cont = step();
		if(!cont)
			break;
	}
}

template<class ModelType>
bool Sampler<ModelType>::step()
{
	bool cont = true;

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

	// Accumulate visits, exceeds
	int index = indices[which];
	while(index < static_cast<int>(levels.size()) - 1)
	{
		bool exceeds = levels[index+1].get_cutoff() < logL[which];
		levels[index].incrementVisits(exceeds);
		if(!exceeds)
			break;
		index++;
	}

	// Accumulate likelihoods for making a new level
	if(static_cast<int>(levels.size()) < options.maxNumLevels &&
			levels.back().get_cutoff() < logL[which])
		logLKeep.push_back(logL[which]);

	// Actually create a new level
	if(static_cast<int>(logLKeep.size()) >= options.newLevelInterval)
	{
		sort(logLKeep.begin(), logLKeep.end());
		int ii = static_cast<int>(0.63212056
				*static_cast<int>(logLKeep.size()));
		LikelihoodType cutoff = logLKeep[ii];
		cout<<setprecision(10);
		cout<<"# Creating level "<<levels.size()
			<<" with logL = "<<cutoff.logL
			<<"."<<endl;
		levels.push_back(Level(levels.back().get_logX() - 1., cutoff));

		if(static_cast<int>(levels.size()) == options.maxNumLevels)
		{
			logLKeep.clear();
			Level::renormaliseVisits(levels, options.newLevelInterval);
		}
		else
			logLKeep.erase(logLKeep.begin(), logLKeep.begin() + ii + 1);

		Level::recalculateLogX(levels, options.newLevelInterval);
		saveLevels();
		deleteParticle();
	}

	if(count%options.saveInterval == 0)
	{
		saveParticle(which);
		Level::recalculateLogX(levels, options.newLevelInterval);
		saveLevels();
		if(options.maxNumSamples > 0 &&
			count/options.saveInterval == options.maxNumSamples)
			cont = false;
	}

	return cont;
}

template<class ModelType>
void Sampler<ModelType>::saveParticle(int which) const
{
	int N = count/options.saveInterval;
	cout<<"# Saving a particle to disk. N = "<<N<<"."<<endl;

	// Save the particle to file
	fstream fout;
	if(N == 1)
	{
		fout.open(options.sampleFile.c_str(), ios::out);
		fout<<"# Samples file. One sample per line."<<endl;
	}
	else
		fout.open(options.sampleFile.c_str(), ios::out|ios::app);
	fout<<setprecision(10);
	particles[which].print(fout); fout<<endl;
	fout.close();

	// Save the particle's info
	if(N == 1)
	{
		fout.open(options.sampleInfoFile.c_str(), ios::out);
		fout<<"# index, logLikelihood, tieBreaker, ID."<<endl;
	}
	else
		fout.open(options.sampleInfoFile.c_str(), ios::out|ios::app);
	fout<<setprecision(10);
	fout<<indices[which]<<' '<<logL[which].logL<<' '
			<<logL[which].tieBreaker<<' '<<which<<endl;
	fout.close();
}

template<class ModelType>
void Sampler<ModelType>::saveLevels() const
{
	fstream fout(options.levelsFile.c_str(), ios::out);
	fout<<"# logX, logLikelihood, tieBreaker, accepts, tries, exceeds, visits."<<endl;
	fout<<setprecision(10);
	for(size_t i=0; i<levels.size(); i++)
		fout<<levels[i]<<endl;
	fout.close();
}

template<class ModelType>
void Sampler<ModelType>::updateParticle(int which)
{
	// Copy the particle
	ModelType proposal = particles[which];
	LikelihoodType logL_proposal = logL[which];

	// Perturb the proposal particle
	double logH = proposal.perturb();
	logL_proposal.logL = proposal.logLikelihood();
	logL_proposal.tieBreaker += pow(10., 1.5 - 6.*randomU())*randn();
	logL_proposal.tieBreaker = mod(logL_proposal.tieBreaker, 1.);
	if(logH > 0.)
		logH = 0.;

	bool accepted = false;
	if(levels[indices[which]].get_cutoff() < logL_proposal
		&& randomU() <= exp(logH))
	{
		// Accept
		particles[which] = proposal;
		logL[which] = logL_proposal;
		accepted = true;
	}
	levels[indices[which]].incrementTries(accepted);
}

template<class ModelType>
void Sampler<ModelType>::updateIndex(int which)
{
	int proposedIndex = indices[which]
		+ static_cast<int>(round(pow(10., 2.0*randomU())*randn()));

	if(proposedIndex == indices[which])
	{
		proposedIndex = (randomU() < 0.5)?(indices[which]+1)
				:(indices[which]-1);
	}

	if(proposedIndex < 0 ||
		proposedIndex >= static_cast<int>(levels.size()))
		return;

	// Acceptance probability. logX part
	double logA = levels[indices[which]].get_logX()
			- levels[proposedIndex].get_logX();

	// Pushing up part
	logA += logPush(proposedIndex) - logPush(indices[which]);

	// Enforce uniform exploration part (if all levels exist)
	if(static_cast<int>(levels.size()) == options.maxNumLevels)
		logA += options.beta*log((double)(levels[indices[which]].get_tries() + 1)/(double)(levels[proposedIndex].get_tries() + 1));

	// Prevent exponentiation of huge numbers
	if(logA > 0.)
		logA = 0.;

	if(randomU() <= exp(logA) && levels[proposedIndex].get_cutoff() < logL[which])
	{
		// Accept!
		indices[which] = proposedIndex;
	}
}

template<class ModelType>
double Sampler<ModelType>::logPush(int index) const
{
	assert(index >= 0 && index < static_cast<int>(levels.size()));
	if(static_cast<int>(levels.size()) == options.maxNumLevels)
		return 0.;

	int i = index - (static_cast<int>(levels.size()) - 1);
	return static_cast<double>(i)/options.lambda;
}

template<class ModelType>
void Sampler<ModelType>::deleteParticle()
{
	// Flag each particle as good or bad
	std::vector<bool> good(options.numParticles, true);
	int numBad = 0;
	for(int i=0; i<options.numParticles; i++)
	{
		if(logPush(indices[i]) <= -5.)
		{
			good[i] = false;
			numBad++;
		}
	}

	if(numBad <= options.numParticles)
	{
		// Replace bad particles with copies of good ones
		for(int i=0; i<options.numParticles; i++)
		{
			if(!good[i])
			{
				int copy;
				do
				{
					copy = randInt(options.numParticles);
				}while(!good[copy]);

				particles[i] = particles[copy];
				indices[i] = indices[copy];
				logL[i] = logL[copy];
				cout<<"# Deleting a particle. Replacing"<<
				" it with a copy of a good survivor."<<endl;
			}
		}
	}
	else
		cerr<<"# Warning: all particles lagging! Very rare!"<<endl;
}

} // namespace DNest3

