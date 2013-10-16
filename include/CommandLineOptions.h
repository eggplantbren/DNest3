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

#ifndef DNest3_CommandLineOptions_h
#define DNest3_CommandLineOptions_h

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
		std::string optionsFile;
		std::string seed;
		std::string dataFile;
		std::string compression;
		int numThreads;

	public:
		CommandLineOptions(int argc, char** argv);

		// Getters
		const std::string& get_levelsFile() const
		{ return levelsFile; }

		const std::string& get_optionsFile() const
		{ return optionsFile; }

		const std::string& get_seed() const
		{ return seed; }

		const std::string& get_dataFile() const
		{ return dataFile; }

		const std::string& get_compression() const
		{ return compression; }

		int get_numThreads() const
		{ return numThreads; }

		// Convert seed string to an integer and return it
		unsigned long get_seed_long() const;

		// Convert compression to a double and return it
		double get_compression_double() const;

		// Print help message
		void printHelp() const;

};

} // namespace DNest3

#endif

