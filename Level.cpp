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

#include "Level.h"
#include "Model.h"
#include <cassert>
#include <cmath>
#include <iomanip>

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

void Level::recalculateLogX(vector<Level>& levels, int regularisation)
{
	assert(levels.size() > 0);

	levels[0].logX = 0.0;
	for(size_t i=1; i<levels.size(); i++)
	{
		levels[i].logX = levels[i-1].logX + log((double)(levels[i-1].exceeds + exp(-1.0)*regularisation)/(double)(levels[i-1].visits + regularisation));
	}
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

void Level::renormaliseVisits(vector<Level>& levels, int regularisation)
{
	for(size_t i=0; i<levels.size(); i++)
	{
		if(i != levels.size() - 1)
			levels[i].renormaliseVisits(regularisation);
		else
		{
			levels[i].accepts = 0;
			levels[i].exceeds = 0;
			levels[i].tries = 0;
			levels[i].visits = 0;
		}
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

} // namespace DNest3

