/*
* Copyright (c) 2009, 2010, 2011, 2012 Brendon J. Brewer.
*
* This file is part of DNest3.
*
* DNest3 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DNest3 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DNest3. If not, see <http://www.gnu.org/licenses/>.
*/

#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <boost/bind.hpp>

namespace DNest3
{

template<class ModelType>
const int MTSampler<ModelType>::skip = 1000;

template<class ModelType>
MTSampler<ModelType>::MTSampler(int numThreads, const Options& options)
:barrier(new boost::barrier(numThreads))
,numThreads(numThreads)
,options(options)
,particles(numThreads, std::vector<ModelType>(options.numParticles))
,logL(numThreads, std::vector<LikelihoodType>(options.numParticles))
,indices(numThreads, std::vector<int>(options.numParticles, 0))
,levels(numThreads, std::vector<Level>(1, Level(0., -1E300, 0.)))
,_levels(1, levels[0][0])
,logLKeep(numThreads)
,initialised(numThreads, false)
,count(numThreads, 0)
,lastSave(0)
,saves(0)
{
	for(int i=0; i<numThreads; i++)
		logLKeep[i].reserve(2*options.newLevelInterval);
}

template<class ModelType>
MTSampler<ModelType>::~MTSampler()
{
	delete barrier;
}

template<class ModelType>
void MTSampler<ModelType>::loadLevels(const char* filename)
{
	_levels = Level::loadLevels(filename);
	if(static_cast<int>(_levels.size()) > options.maxNumLevels)
	{
		std::cout<<"# Truncating to "<<options.maxNumLevels<<" levels."<<std::endl;
		_levels.erase(_levels.begin() + options.maxNumLevels, _levels.end());
	} 

	Level::renormaliseVisits(_levels, options.newLevelInterval);
	Level::recalculateLogX(_levels, options.newLevelInterval);
	saveLevels();

	for(int i=0; i<numThreads; i++)
		levels[i] = _levels;
}

template<class ModelType>
void MTSampler<ModelType>::initialise(int thread)
{
	static boost::mutex mutex;

	assert(!initialised[thread]);
	for(int i=0; i<options.numParticles; i++)
	{
		particles[thread][i].fromPrior();
		logL[thread][i] = LikelihoodType
				(particles[thread][i].logLikelihood(), randomU());
		logLKeep[thread].push_back(logL[thread][i]);
	}
	initialised[thread] = true;
	mutex.lock();
	std::cout<<"# Thread "<<(thread+1)<<": Generated "<<options.numParticles<<
			" particles from the prior."<<std::endl;
	mutex.unlock();
}

template<class ModelType>
void MTSampler<ModelType>::run()
{
	
	boost::thread_group threads;
	for(int i=0; i<numThreads; i++)
	{
		threads.create_thread(boost::bind(
			&MTSampler<ModelType>::runThread,
			this, i, RandomNumberGenerator::get_instance().get_seed()));
	}
	threads.join_all();
}

template<class ModelType>
void MTSampler<ModelType>::runThread(int thread, unsigned long firstSeed)
{
	// Re-seed thread-local RNG
	RandomNumberGenerator::initialise_instance();
	RandomNumberGenerator::get_instance().set_seed(firstSeed + 100*thread);

	if(!initialised[thread])
		initialise(thread);

	bool cont = true;
	while(cont)
	{
		steps(thread, skip);
		barrier->wait();
		if(thread == 0)
		{
			lastSave += numThreads*skip;
			cont = bookKeeping();
		}
		barrier->wait();
	}
}


template<class ModelType>
void MTSampler<ModelType>::steps(int thread, int steps)
{
	if(!initialised[thread])
		initialise(thread);

	for(int i=0; i<steps; i++)
		step(thread);
}

template<class ModelType>
void MTSampler<ModelType>::step(int thread)
{
	// Move a particle
	int which = randInt(options.numParticles);
	if(randomU() <= 0.5)
	{
		updateParticle(thread, which);
		updateIndex(thread, which);
	}
	else
	{
		updateIndex(thread, which);
		updateParticle(thread, which);
	}
	count[thread]++;

	// Accumulate visits, exceeds
	int index = indices[thread][which];
	while(index < static_cast<int>(levels[thread].size()) - 1)
	{
		bool exceeds = levels[thread][index+1].get_cutoff() < logL[thread][which];
		levels[thread][index].incrementVisits(exceeds);
		if(!exceeds)
			break;
		index++;
	}

	// Accumulate likelihoods for making a new level
	if(static_cast<int>(levels[thread].size()) < options.maxNumLevels &&
			levels[thread].back().get_cutoff() < logL[thread][which])
		logLKeep[thread].push_back(logL[thread][which]);
}


template<class ModelType>
bool MTSampler<ModelType>::bookKeeping()
{
	bool cont = true;
/*
	// Actually create a new level
	if(static_cast<int>(logLKeep.size()) >= options.newLevelInterval)
	{
		sort(logLKeep.begin(), logLKeep.end());
		int ii = static_cast<int>(0.63212056
				*static_cast<int>(logLKeep.size()));
		LikelihoodType cutoff = logLKeep[ii];
		std::cout<<std::setprecision(10);
		std::cout<<"# Creating level "<<levels.size()
			<<" with logL = "<<cutoff.logL
			<<"."<<std::endl;
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
*/

	int iWhich = randInt(numThreads);
	int jWhich = randInt(options.numParticles);

	if(lastSave >= options.saveInterval)
	{
		saveParticle(iWhich, jWhich);
		Level::recalculateLogX(_levels, options.newLevelInterval);
		saveLevels();
		if(options.maxNumSamples > 0 &&
			saves >= options.maxNumSamples)
			cont = false;
	}

	return cont;
}

template<class ModelType>
void MTSampler<ModelType>::saveParticle(int iWhich, int jWhich)
{
	std::cout<<"# Saving a particle to disk. N = "<<(++saves)<<"."<<std::endl;

	// Save the particle to file
	std::fstream fout;
	if(saves == 1)
	{
		fout.open(options.sampleFile.c_str(), std::ios::out);
		fout<<"# Samples file. One sample per line."<<std::endl;
		fout<<"# "<<particles[0][0].description()<<std::endl;
	}
	else
		fout.open(options.sampleFile.c_str(), std::ios::out|std::ios::app);
	fout<<std::setprecision(10);
	particles[iWhich][jWhich].print(fout); fout<<std::endl;
	fout.close();

	// Save the particle's info
	if(saves == 1)
	{
		fout.open(options.sampleInfoFile.c_str(), std::ios::out);
		fout<<"# index, logLikelihood, tieBreaker, ID."<<std::endl;
	}
	else
		fout.open(options.sampleInfoFile.c_str(), std::ios::out|std::ios::app);
	fout<<std::setprecision(10);
	fout<<indices[iWhich][jWhich]<<' '<<logL[iWhich][jWhich].logL<<' '
			<<logL[iWhich][jWhich].tieBreaker<<' '
			<<(iWhich*options.numParticles + jWhich)<<std::endl;
	fout.close();
	lastSave = 0;
}

template<class ModelType>
void MTSampler<ModelType>::saveLevels() const
{
	std::fstream fout(options.levelsFile.c_str(), std::ios::out);
	fout<<"# logX, logLikelihood, tieBreaker, accepts, tries, exceeds, visits."<<std::endl;
	fout<<std::setprecision(10);
	for(size_t i=0; i<_levels.size(); i++)
		fout<<_levels[i]<<std::endl;
	fout.close();
}

template<class ModelType>
void MTSampler<ModelType>::updateParticle(int thread, int which)
{
	// Copy the particle
	ModelType proposal = particles[thread][which];
	LikelihoodType logL_proposal = logL[thread][which];

	// Perturb the proposal particle
	double logH = proposal.perturb();
	logL_proposal.logL = proposal.logLikelihood();
	logL_proposal.tieBreaker += pow(10., 1.5 - 6.*randomU())*randn();
	logL_proposal.tieBreaker = mod(logL_proposal.tieBreaker, 1.);
	if(logH > 0.)
		logH = 0.;

	bool accepted = false;
	if(levels[thread][indices[thread][which]].get_cutoff() < logL_proposal
		&& randomU() <= exp(logH))
	{
		// Accept
		particles[thread][which] = proposal;
		logL[thread][which] = logL_proposal;
		accepted = true;
	}
	levels[thread][indices[thread][which]].incrementTries(accepted);
}

template<class ModelType>
void MTSampler<ModelType>::updateIndex(int thread, int which)
{
	int proposedIndex = indices[thread][which]
		+ static_cast<int>(round(pow(10., 2.0*randomU())*randn()));

	if(proposedIndex == indices[thread][which])
	{
		proposedIndex = (randomU() < 0.5)?(indices[thread][which]+1)
				:(indices[thread][which]-1);
	}

	if(proposedIndex < 0 ||
		proposedIndex >= static_cast<int>(levels[thread].size()))
		return;

	// Acceptance probability. logX part
	double logA = levels[thread][indices[thread][which]].get_logX()
			- levels[thread][proposedIndex].get_logX();

	// Pushing up part
	logA += logPush(proposedIndex) - logPush(indices[thread][which]);

	// Enforce uniform exploration part (if all levels exist)
	if(static_cast<int>(levels[thread].size()) == options.maxNumLevels)
		logA += options.beta*log((double)(levels[thread][indices[thread][which]].get_tries() + 1)/(double)(levels[thread][proposedIndex].get_tries() + 1));

	// Prevent exponentiation of huge numbers
	if(logA > 0.)
		logA = 0.;

	if(randomU() <= exp(logA) && levels[thread][proposedIndex].get_cutoff() < logL[thread][which])
	{
		// Accept!
		indices[thread][which] = proposedIndex;
	}
}

template<class ModelType>
double MTSampler<ModelType>::logPush(int index) const
{
	assert(index >= 0 && index < static_cast<int>(_levels.size()));
	if(static_cast<int>(_levels.size()) == options.maxNumLevels)
		return 0.;

	int i = index - (static_cast<int>(_levels.size()) - 1);
	return static_cast<double>(i)/options.lambda;
}

template<class ModelType>
void MTSampler<ModelType>::deleteParticle()
{
	// Flag each particle as good or bad
	std::vector< std::vector<bool> > good(numThreads, std::vector<bool>(options.numParticles, true));
	int numBad = 0;
	for(int i=0; i<numThreads; i++)
	{
		for(int j=0; j<options.numParticles; j++)
		{
			if(logPush(indices[i][j]) <= -5.)
			{
				good[i][j] = false;
				numBad++;
			}
		}
	}

	if(numBad < numThreads*options.numParticles)
	{
		// Replace bad particles with copies of good ones
		for(int i=0; i<numThreads; i++)
		{
			for(int j=0; j<options.numParticles; j++)
			{
				if(!good[i][j])
				{
					int iCopy, jCopy;
					do
					{
						iCopy = randInt(numThreads);
						jCopy = randInt(options.numParticles);
					}while(!good[iCopy][jCopy]);

					particles[i][j] = particles[iCopy][jCopy];
					indices[i][j] = indices[iCopy][jCopy];
					logL[i][j] = logL[iCopy][jCopy];
					std::cout<<"# Deleting a particle. Replacing"<<
					" it with a copy of a good survivor."<<std::endl;
				}
			}
		}
	}
	else
		std::cerr<<"# Warning: all particles lagging! Very rare!"<<std::endl;
}

} // namespace DNest3

