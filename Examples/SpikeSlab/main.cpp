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
#include "Sampler.h"
#include "SpikeSlab.h"
#include "RandomNumberGenerator.h"

using namespace std;
using namespace DNest3;

int main(int argc, char** argv)
{
	Options options("OPTIONS");
	RandomNumberGenerator::get_instance().setSeed(-time(0));

	Sampler<SpikeSlab> sampler(options);
	if(argc >= 2)
		sampler.loadLevels(argv[1]);
	sampler.run();

	return 0;
}

