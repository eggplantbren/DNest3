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

#include <cstdlib>
#include <iostream>
#include "CommandLineOptions.h"
#include "RandomNumberGenerator.h"
#include "Sampler.h"
#include "Options.h"

namespace DNest3
{

template<class ModelType>
Sampler<ModelType> setup(int argc, char** argv)
{
	CommandLineOptions options(argc, argv);
	return setup<ModelType>(options);
}

template<class ModelType>
Sampler<ModelType> setup(const CommandLineOptions& options)
{
	// Get number of threads, print messages
	if(options.get_numThreads() > 1)
	{
		std::cerr<<"Multithreading not supported."<<std::endl;
		exit(0);
	}
	std::cout<<"# Using "<<options.get_numThreads()<<" thread"<<
		((options.get_numThreads() == 1)?("."):("s."))<<std::endl;

	// Seed random number generator
	std::cout<<"# Seeding random number generator with "<<
		options.get_seed_int()<<"."<<std::endl;
	RandomNumberGenerator::get_instance().setSeed(options.get_seed_int());

	// Load sampler options from file
	Options samplerOptions(options.get_optionsFile().c_str());

	// Create sampler
	Sampler<ModelType> sampler(samplerOptions);

	// Load levels file if requested
	if(options.get_levelsFile().compare("") != 0)
		sampler.loadLevels(options.get_levelsFile().c_str());

	return sampler;
}

template<class ModelType>
void start(int argc, char** argv)
{
	CommandLineOptions options(argc, argv);
	Sampler<ModelType> sampler =
			setup<ModelType>(options);
	sampler.run();
}

} // namespace DNest3

