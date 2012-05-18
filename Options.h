/*
* Copyright (c) 2009, 2010, 2011, 2012 Brendon J. Brewer.
*
* This file is part of DNest.
*
* DNest is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DNest is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DNest. If not, see <http://www.gnu.org/licenses/>.
*/

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

