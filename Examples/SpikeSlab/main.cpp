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

#include <iostream>
#include <ctime>
#include "CommandLineOptions.h"
#include "Sampler.h"
#include "SpikeSlab.h"
#include "RandomNumberGenerator.h"

using namespace std;
using namespace DNest3;

int main(int argc, char** argv)
{
	// Handle command line options
	CommandLineOptions options(argc, argv);
	cout<<"# Seeding random number generator with "<<
		options.get_seed_int()<<"."<<endl;
	RandomNumberGenerator::get_instance().setSeed(options.get_seed_int());

	// Get number of threads, print messages
	if(options.get_numThreads() > 1)
	{
		cerr<<"Multithreading not supported."<<endl;
		exit(0);
	}
	cout<<"# Using "<<options.get_numThreads()<<" thread"<<
		((options.get_numThreads() == 1)?("."):("s."))<<endl;

	// Load sampler options from file
	Options samplerOptions("OPTIONS");

	// Create sampler
	Sampler<SpikeSlab> sampler(samplerOptions);

	// Load levels file if requested
	if(options.get_levelsFile().compare("") != 0)
		sampler.loadLevels(options.get_levelsFile().c_str());

	// Sample!
	sampler.run();

	return 0;
}

