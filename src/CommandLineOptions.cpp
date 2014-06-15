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

#include "CommandLineOptions.h"
#include <ctime>
#include <sstream>
#include <iostream>
#include <unistd.h> // for getopt
#include <cstdlib> // for exit

using namespace std;

namespace DNest3
{

CommandLineOptions::CommandLineOptions(int argc, char** argv)
:levelsFile("")
,optionsFile("OPTIONS")
,seed("time")
,dataFile("")
,compression("2.7182818284590451")
,numThreads(1)
{
	// The following code is based on the example given at
	// http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt

	int c;
	stringstream s;

	opterr = 0;
	while((c = getopt(argc, argv, "hl:o:s:d:c:t:")) != -1)
	switch(c)
	{
		case 'h':
			printHelp();
			break;
		case 'l':
			levelsFile = string(optarg);
			break;
		case 'o':
			optionsFile = string(optarg);
			break;
		case 's':
			seed = string(optarg);
			break;
		case 'd':
			dataFile = string(optarg);
			break;
		case 'c':
			compression = string(optarg);
			break;
		case 't':
			s<<optarg;
			s>>numThreads;
			break;
		case '?':
			cerr<<"# Option "<<optopt<<" requires an argument."<<endl;
			if(isprint(optopt))
				cerr<<"# Unknown option "<<optopt<<"."<<endl;
			else
				cerr<<"# Unknown option character "<<optopt<<"."<<endl;
			exit(0);
		default:
			abort();
           }

	for(int index = optind; index < argc; index++)
		cout<<"# Non-option argument "<<argv[index]<<endl;

	if(numThreads <= 0)
	{
		cerr<<"# Invalid number of threads: "<<numThreads<<"."<<endl;
		numThreads = 1;
	}
}

double CommandLineOptions::get_compression_double() const
{
	double val;
	stringstream s(compression);
	s>>val;
	return val;
}

unsigned long CommandLineOptions::get_seed_long() const
{
	if(seed.compare("time") == 0)
		return static_cast<unsigned long>(time(0));

	unsigned long i;
	stringstream s(seed);
	s>>i;
	return i;
}

void CommandLineOptions::printHelp() const
{
	cout<<"DNest3 Command Line Options: "<<endl;
	cout<<"-h: display this message"<<endl;
	cout<<"-l <filename>: load level structure from the specified file."<<endl;
	cout<<"-o <filename>: load DNest3 options from the specified file. Default=OPTIONS"<<endl;
	cout<<"-s <seed>: seed the random number generator with the specified value. If unspecified, the system time is used."<<endl;
	cout<<"-d <filename>: Load data from the specified file, if required."<<endl;
	cout<<"-c <value>: Specify a compression value (between levels) other than e."<<endl;
	cout<<"-t <num_threads>: run on the specified number of threads. Default=1."<<endl;
	exit(0);
}

} // namespace DNest3

