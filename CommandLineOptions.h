#ifndef _CommandLineOptions_
#define _CommandLineOptions_

#include <string>

namespace DNest3
{

/*
* A little class containing all of the command line options.
* Uses GNU GetOpt to parse the input.
*/

class CommandLineOptions
{
	private:
		std::string levelsFile;
		std::string seed;
		int numThreads;

	public:
		CommandLineOptions(int argc, char** argv);

		// Getters
		const std::string& get_levelsFile() const
		{ return levelsFile; }

		const std::string& get_seed() const
		{ return seed; }

		int get_numThreads() const
		{ return numThreads; }

		// Convert seed string to an integer and return it
		int get_seed_int() const;

};

} // namespace DNest3

#endif

