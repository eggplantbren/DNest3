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

#include "Data.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdlib>

using namespace std;

Data Data::instance;

Data::Data()
:loaded(false)
{

}

void Data::load(const char* filename)
{
	x.clear();
	y.clear();
	sig.clear();

	fstream fin(filename, ios::in);
	if(!fin)
	{
		cerr<<"# ERROR: Cannot open file "<<filename<<"."<<endl;
		exit(0);
	}

	// Skip comment lines at the top of the file
	while(fin.peek() == '#')
		fin.ignore(1000000, '\n');

	// Read in data
	double temp1, temp2, temp3;
	while(fin>>temp1 && fin>>temp2 && fin>>temp3)
	{
		x.push_back(temp1);
		y.push_back(temp2);
		sig.push_back(temp3);
	}
	cout<<"# Loaded "<<x.size()<<" points from file "<<filename<<"."<<endl;
	fin.close();

	loaded = true;
}

