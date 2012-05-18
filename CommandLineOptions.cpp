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
,seed("time")
,numThreads(1)
{
	// The following code is based on the example given at
	// http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt

	int c;
	stringstream s;

	opterr = 0;
	while((c = getopt(argc, argv, "lst:")) != -1)
	switch(c)
	{
		case 'l':
			levelsFile = string(optarg);
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
}

int CommandLineOptions::get_seed_int() const
{
	if(seed.compare("time") == 0)
		return -time(0);

	int i;
	stringstream s(seed);
	s>>i;
	return i;
}

} // namespace DNest3

