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

#ifndef DNest3_Sampler_h
#define DNest3_Sampler_h

#include <vector>
#include "LikelihoodType.h"
#include "Options.h"
#include "Level.h"

namespace DNest3
{

template<class ModelType>
class Sampler
{
	private:
		// Options (most useful comment ever)
		Options options;

		// Target compression value
		double compression;

		// Stuff pertaining to the particles
		std::vector<ModelType> particles;
		std::vector<LikelihoodType> logL;
		std::vector<int> indices;

		// Stuff pertaining to the level structure
		std::vector<Level> levels;
		std::vector<LikelihoodType> logLKeep;

		// Whether initialise() has ever been called
		bool initialised;

		// Number of MCMC steps ever done
		long count;

	public:
		// Constructor: Pass in Options object
		Sampler(double compression, const Options& options);

		// Load levels from file
		void loadLevels(const char* filename);

		// Do the sampling!
		void run();

	private:
		// These are helper methods -- not part of the public interface

		// Initialise all objects from the prior
		void initialise();

		// run() but only for a certain number of steps
		void run(int steps);

		// Choose a particle and do one step with it
		// Return a flag - whether to continue or not
		bool step();

		// Do a M-H step of a particle
		void updateParticle(int which);

		// Do a M-H step of an index
		void updateIndex(int which);

		// Check for creation of a new level, saving of particles, etc
		bool bookKeeping(int which);

		// Savers
		void saveParticle(int which) const;
		void saveLevels() const;

		// Pushing function
		double logPush(int index) const;

		// Delete any bad particles
		void deleteParticle();
};

}

#include "SamplerImpl.h"

#endif

