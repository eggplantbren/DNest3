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
MTSampler<ModelType>::MTSampler(int numThreads, double compression, const Options& options)
:barrier(new boost::barrier(numThreads))
,numThreads(numThreads)
,compression(compression)
,options(options)
,particles(numThreads, std::vector<ModelType>(options.numParticles))
,logL(numThreads, std::vector<LikelihoodType>(options.numParticles))
,indices(numThreads, std::vector<int>(options.numParticles, 0))
,levels(numThreads, std::vector<Level>(1, Level(0., -1E300, 0.)))
,logLKeep(numThreads)
,_levels(1, levels[0][0])
,count(numThreads, 0)
,lastSave(0)
,saves(0)
,deletions(0)
,cont(true)
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
	Level::recalculateLogX(_levels, compression, 100);
	saveLevels();

	for(int i=0; i<numThreads; i++)
		levels[i] = _levels;
}

template<class ModelType>
void MTSampler<ModelType>::initialise(int thread)
{
	static boost::mutex mutex;
	for(int i=0; i<options.numParticles; i++)
	{
		particles[thread][i].fromPrior();
		logL[thread][i] = LikelihoodType
				(particles[thread][i].logLikelihood(), randomU());
		logLKeep[thread].push_back(logL[thread][i]);
	}
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
	RandomNumberGenerator::get_instance().set_seed(firstSeed + 100*(thread+1));

	initialise(thread);

	while(cont)
	{
		steps(thread, options.threadSteps);
		barrier->wait();
		if(thread == 0)
		{
			lastSave += numThreads*options.threadSteps;
			cont = bookKeeping();
		}
		barrier->wait();
	}
}


template<class ModelType>
void MTSampler<ModelType>::steps(int thread, int steps)
{
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
void MTSampler<ModelType>::shuffle()
{
	// No need to do it if there's only one particle
	if(options.numParticles*numThreads <= 1)
		return;

	for(int i=0; i<options.numParticles*numThreads; i++)
	{
		// Choose to particles to swap
		int i1, j1, i2, j2;
		do
		{
			i1 = randInt(numThreads);
			j1 = randInt(options.numParticles);
			i2 = randInt(numThreads);
			j2 = randInt(options.numParticles);
		}while(i1 == i2 && j1 == j2);


		// Do the swap
		ModelType temp1 = particles[i1][j1];
		LikelihoodType temp2 = logL[i1][j1];
		int temp3 = indices[i1][j1];

		particles[i1][j1] = particles[i2][j2];
		logL[i1][j1] = logL[i2][j2];
		indices[i1][j1] = indices[i2][j2];

		particles[i2][j2] = temp1;
		logL[i2][j2] = temp2;
		indices[i2][j2] = temp3;
	}
}

template<class ModelType>
bool MTSampler<ModelType>::bookKeeping()
{
	shuffle();

	bool cont = true;

	createLevel();
	gatherLevels();
	broadcastLevels();

	int iWhich = randInt(numThreads);
	int jWhich = randInt(options.numParticles);
	if(lastSave >= options.saveInterval)
	{
		saveParticle(iWhich, jWhich);
		saveLevels();
		if(options.maxNumSamples > 0 &&
			saves >= options.maxNumSamples)
			cont = false;
		lastSave = 0;
	}

	return cont;
}

template<class ModelType>
void MTSampler<ModelType>::gatherLevels()
{
	// Add new statistics to _levels
	std::vector<Level> old = _levels;
	for(int i=0; i<numThreads; i++)
	{
		for(size_t j=0; j<levels[i].size(); j++)
		{
			_levels[j] += levels[i][j];
			_levels[j] -= old[j];
		}
	}
	Level::recalculateLogX(_levels, compression, 100);
}

template<class ModelType>
void MTSampler<ModelType>::broadcastLevels()
{
	// Send out _levels to each thread
	for(int i=0; i<numThreads; i++)
		levels[i] = _levels;
}

template<class ModelType>
void MTSampler<ModelType>::createLevel()
{
	// If all levels exist, exit this method
	if(static_cast<int>(_levels.size()) >= options.maxNumLevels)
		return;

	// Count logLKeep
	long tot = 0;
	for(int i=0; i<numThreads; i++)
		tot += logLKeep[i].size();

	// If not enough, exit this method
	if(tot < options.newLevelInterval)
		return;

	std::vector<LikelihoodType> giant(tot);
	unsigned long k = 0;
	for(int i=0; i<numThreads; i++)
		for(size_t j=0; j<logLKeep[i].size(); j++)
			giant[k++] = logLKeep[i][j];

	sort(giant.begin(), giant.end());
	int ii = static_cast<int>((1. - 1./compression)
			*static_cast<int>(giant.size()));
	LikelihoodType cutoff = giant[ii];
	std::cout<<std::setprecision(10);
	std::cout<<"# Creating level "<<_levels.size()
			<<" with logL = "<<cutoff.logL
			<<"."<<std::endl;
	_levels.push_back(Level(_levels.back().get_logX() - log(compression), cutoff));

	if(static_cast<int>(_levels.size()) == options.maxNumLevels)
	{
		gatherLevels();
		Level::renormaliseVisits(_levels, options.newLevelInterval);
		broadcastLevels();
		for(int i=0; i<numThreads; i++)
			logLKeep[i].clear();
	}
	else
	{
		giant.erase(giant.begin(), giant.begin() + ii + 1);
		for(int i=0; i<numThreads; i++)
		{
			logLKeep[i].clear();
			logLKeep[i].reserve(2*options.newLevelInterval);
		}
		logLKeep[0] = giant;
	}
	saveLevels();
	deleteParticle();
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
	double logH = 0.;

	// Standard Metropolis move
	logH = proposal.perturb();
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

	// How good is the best particle?
	double max_logPush = -1E300;

	int numBad = 0;
	for(int i=0; i<numThreads; i++)
	{
		for(int j=0; j<options.numParticles; j++)
		{
			if(logPush(indices[i][j]) > max_logPush)
				max_logPush = logPush(indices[i][j]);
			if(logPush(indices[i][j]) < -5.)
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
					// Choose a replacement particle. Higher prob
					// of selecting better particles.
					int iCopy, jCopy;
					do
					{
						iCopy = randInt(numThreads);
						jCopy = randInt(options.numParticles);
					}while(!good[iCopy][jCopy] || randomU() >= exp(logPush(indices[i][j]) - max_logPush));

					particles[i][j] = particles[iCopy][jCopy];
					indices[i][j] = indices[iCopy][jCopy];
					logL[i][j] = logL[iCopy][jCopy];
					deletions++;

					std::cout<<"# Replacing languishing particle.";
					std::cout<<" This has happened "<<deletions;
					std::cout<<" times."<<std::endl;
				}
			}
		}
	}
	else
		std::cerr<<"# Warning: all particles lagging! Very rare!"<<std::endl;
}

} // namespace DNest3

