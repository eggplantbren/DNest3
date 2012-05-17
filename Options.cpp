#include "Options.h"
#include "Utils.h"
#include <fstream>
#include <iostream>

using namespace std;

namespace DNest3
{

Options::Options(	unsigned int numParticles,
			unsigned int newLevelInterval,
			unsigned int saveInterval,
			unsigned int maxNumLevels,
			double lambda,
			double beta,
			unsigned int maxNumSamples)
:numParticles(numParticles)
,newLevelInterval(newLevelInterval)
,saveInterval(saveInterval)
,maxNumLevels(maxNumLevels)
,lambda(lambda)
,beta(beta)
,maxNumSamples(maxNumSamples)
{

}

Options::Options(const char* filename)
{
	load(filename);
}

void Options::load(const char* filename)
{
	fstream fin(filename, ios::in);
	if(!fin)
		cerr<<"ERROR: Cannot open file "<<filename<<"."<<endl;

	// Read past comment lines at the top of the file
	while(fin.peek() == '#')
		fin.ignore(1000000, '\n');

	fin>>numParticles;	fin.ignore(1000000, '\n');
	fin>>newLevelInterval;	fin.ignore(1000000, '\n');
	fin>>saveInterval;	fin.ignore(1000000, '\n');
	fin>>maxNumLevels;	fin.ignore(1000000, '\n');
	fin>>lambda;		fin.ignore(1000000, '\n');
	fin>>beta;		fin.ignore(1000000, '\n');
	fin>>maxNumSamples;

	fin.close();
}

} // namespace DNest3

