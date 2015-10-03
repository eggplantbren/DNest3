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

#ifndef DNest3_MTSampler_h
#define DNest3_MTSampler_h

#include <vector>
#include "LikelihoodType.h"
#include "Options.h"
#include "Level.h"
#include <boost/thread.hpp>

namespace DNest3
{

template<class ModelType>
class MTSampler
{
	private:
		// Boost barrier - allocate on heap
		boost::barrier* barrier;

		// Number of threads
		int numThreads;

		// Target compression value
		double compression;

		// Options (most useful comment ever)
		Options options;

		// Stuff pertaining to the particles
		std::vector< std::vector<ModelType> > particles;
		std::vector< std::vector<LikelihoodType> > logL;
		std::vector< std::vector<int> > indices;

		// Stuff pertaining to the level structure
		std::vector< std::vector<Level> > levels;
		std::vector< std::vector<LikelihoodType> > logLKeep;

		// Backed up levels
		std::vector<Level> _levels;

		// Number of MCMC steps ever done
		std::vector<long> count;

		// Number of MCMC steps since last save
		long lastSave;

		// Number of saved particles
		int saves;

		// Number of times a languishing particle has been deleted
		int deletions;

		// Flag - continue?
		bool cont;

	public:
		// Constructor: Pass in Options object
		MTSampler(int numThreads, double compression, const Options& options);

		// Destructor - delete barrier
		~MTSampler();

		// Load levels from file
		void loadLevels(const char* filename);

		// Do the sampling!
		void run();

	private:
		// These are helper methods -- not part of the public interface

		// Initialise all objects from the prior
		void initialise(int thread);

		// The main thing to launch threads with
		void runThread(int thread, unsigned long firstSeed);

		// Like run() but only for a certain number of steps
		void steps(int thread, int steps);

		// Choose a particle and do one step with it
		void step(int thread);

		// Do a M-H step of a particle
		void updateParticle(int thread, int which);

		// Do a M-H step of an index
		void updateIndex(int thread, int which);

		// Check for creation of a new level, saving of particles, etc
		bool bookKeeping();

		// Shuffle particles between threads
		void shuffle();

		// Check for creation of a new level
		void createLevel();

		// Combine info from levels into _levels
		void gatherLevels();

		// and then set all levels to _levels
		void broadcastLevels();

		// Savers
		void saveParticle(int iWhich, int jWhich);
		void saveLevels() const;

		// Pushing function
		double logPush(int index) const;

		// Delete any bad particles
		void deleteParticle();
};

}

#include "MTSamplerImpl.h"

#endif

