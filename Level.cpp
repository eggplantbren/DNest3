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

#include "Level.h"
#include "Model.h"
#include <cassert>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <iostream>

using namespace std;

namespace DNest3
{

Level::Level(double logX, double logL, double tieBreaker)
:logX(logX)
,cutoff(logL, tieBreaker)
,accepts(0), tries(0), visits(0), exceeds(0)
{

}

Level::Level(double logX, const LikelihoodType& cutoff)
:logX(logX)
,cutoff(cutoff)
,accepts(0), tries(0), visits(0), exceeds(0)
{

}

void Level::renormaliseVisits(int regularisation)
{
	if(tries >= regularisation)
	{
		accepts = ((double)(accepts+1)/(double)(tries+1))*regularisation;
		tries = regularisation;
	}

	if(visits >= regularisation)
	{
		exceeds = ((double)(exceeds+1)/(double)(visits+1))*regularisation;
		visits = regularisation;
	}
}

void Level::incrementVisits(bool incrementExceeds)
{
	visits++;
	if(incrementExceeds)
		exceeds++;
}

void Level::incrementTries(bool accepted)
{
	tries++;
	if(accepted)
		accepts++;
}

ostream& operator << (ostream& out, const Level& level)
{
	out<<setprecision(10)<<level.logX<<' '<<level.cutoff.logL<<' '<<level.cutoff.tieBreaker<<' ';
	out<<level.accepts<<' '<<level.tries<<' '<<level.exceeds<<' '<<level.visits<<' ';
	return out;
}

istream& operator >> (istream& in, Level& level)
{
	in>>level.logX>>level.cutoff.logL>>level.cutoff.tieBreaker;
	in>>level.accepts>>level.tries>>level.exceeds>>level.visits;
	return in;
}


void Level::recalculateLogX(vector<Level>& levels, double compression, int regularisation)
{
	assert(levels.size() > 0);

	levels[0].logX = 0.0;
	for(size_t i=1; i<levels.size(); i++)
	{
		levels[i].logX = levels[i-1].logX + log((double)(levels[i-1].exceeds + (1./compression)*regularisation)/(double)(levels[i-1].visits + regularisation));
	}
}

void Level::renormaliseVisits(vector<Level>& levels, int regularisation)
{
	for(size_t i=0; i<levels.size(); i++)
		levels[i].renormaliseVisits(regularisation);
}

vector<Level> Level::loadLevels(const char* filename)
{
	Level level(0., -1E300, 0.);
	vector<Level> levels;

	fstream fin(filename, ios::in);
	if(!fin)
	{
		cerr<<"# WARNING: Couldn't load levels from file "<<filename
			<<"."<<endl;
		levels.push_back(level);
		return levels;
	}
	// Read past comment lines at the top of the file
	while(fin.peek() == '#')
		fin.ignore(1000000, '\n');

	while(fin>>level)
		levels.push_back(level);
	fin.close();

	cout<<"# Found "<<levels.size()<<" levels in file "
		<<filename<<"."<<endl;

	return levels;
}

Level& Level::operator += (const Level& other)
{
	if(logX != other.logX || cutoff.logL != other.cutoff.logL || cutoff.tieBreaker != other.cutoff.tieBreaker)
		cerr<<"# Warning: combining level statistics from non-equivalent levels."<<endl;

	tries += other.tries;
	accepts += other.accepts;
	visits += other.visits;
	exceeds += other.exceeds;

	return *this;
}

Level& Level::operator -= (const Level& other)
{
	if(logX != other.logX || cutoff.logL != other.cutoff.logL || cutoff.tieBreaker != other.cutoff.tieBreaker)
		cerr<<"# Warning: combining level statistics from non-equivalent levels."<<endl;

	tries -= other.tries;
	accepts -= other.accepts;
	visits -= other.visits;
	exceeds -= other.exceeds;

	return *this;
}

} // namespace DNest3

