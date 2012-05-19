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
,numThreads(1)
{
	// The following code is based on the example given at
	// http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt

	int c;
	stringstream s;

	opterr = 0;
	while((c = getopt(argc, argv, "hl:o:s:t:")) != -1)
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

int CommandLineOptions::get_seed_int() const
{
	if(seed.compare("time") == 0)
		return -abs(time(0));

	int i;
	stringstream s(seed);
	s>>i;
	return -abs(i);
}

void CommandLineOptions::printHelp() const
{
	cout<<"DNest 3 Command Line Options: "<<endl;
	cout<<"-h: display this message"<<endl;
	cout<<"-l <filename>: load level structure from the specified file."<<endl;
	cout<<"-o <filename>: load DNest3 options from the specified file. Default=OPTIONS"<<endl;
	cout<<"-s <seed>: seed the random number generator with the specified value. If unspecified, the system time is used."<<endl;
	cout<<"-t <num_threads>: run on the specified number of threads. Default=1."<<endl;
	exit(0);
}


} // namespace DNest3

