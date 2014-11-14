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

#include "Options.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <cassert>
#include <cstdlib>

using namespace std;

namespace DNest3
{

Options::Options(int numParticles,
		 int newLevelInterval,
		 int saveInterval,
		 int threadSteps,
		 int maxNumLevels,
		 double lambda,
		 double beta,
		 int maxNumSamples)
:numParticles(numParticles)
,newLevelInterval(newLevelInterval)
,saveInterval(saveInterval)
,threadSteps(threadSteps)
,maxNumLevels(maxNumLevels)
,lambda(lambda)
,beta(beta)
,maxNumSamples(maxNumSamples)
,sampleFile("sample.txt")
,sampleInfoFile("sample_info.txt")
,levelsFile("levels.txt")
{
	assert( numParticles > 0 && newLevelInterval > 0 &&
		maxNumLevels > 0 && lambda > 0. &&
		beta >= 0. && maxNumSamples >= 0);
}

Options::Options(const char* filename)
:sampleFile("sample.txt")
,sampleInfoFile("sample_info.txt")
,levelsFile("levels.txt")
{
	load(filename);
}

void Options::load(const char* filename)
{
	fstream fin(filename, ios::in);
	if(!fin)
	{
		cerr<<"ERROR: Cannot open file "<<filename<<"."<<endl;
		exit(0);
	}

	// Read past comment lines at the top of the file
	while(fin.peek() == '#')
		fin.ignore(1000000, '\n');

	fin>>numParticles;	fin.ignore(1000000, '\n');
	fin>>newLevelInterval;	fin.ignore(1000000, '\n');
	fin>>saveInterval;	fin.ignore(1000000, '\n');
	fin>>threadSteps;	fin.ignore(1000000, '\n');
	fin>>maxNumLevels;	fin.ignore(1000000, '\n');
	fin>>lambda;		fin.ignore(1000000, '\n');
	fin>>beta;		fin.ignore(1000000, '\n');
	fin>>maxNumSamples;

	fin.close();

	assert( numParticles > 0 && newLevelInterval > 0 &&
	threadSteps > 0 && maxNumLevels > 0 && lambda > 0. &&
	beta >= 0. && maxNumSamples >= 0);
}

} // namespace DNest3

