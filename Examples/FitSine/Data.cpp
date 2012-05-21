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

using namespace std;

Data Data::instance;

Data::Data()
:loaded(false)
{

}

void Data::load(const char* filename)
{
	t.clear();
	y.clear();
	sig.clear();
	inverseVariance.clear();

	fstream fin(filename, ios::in);
	if(!fin)
	{
		cerr<<"# ERROR: Cannot open file "<<filename<<"."<<endl;
		return;
	}

	// Skip comment lines at the top of the file
	while(fin.peek() == '#')
		fin.ignore(1000000, '\n');

	// Read in data
	double temp1, temp2, temp3;
	while(fin>>temp1 && fin>>temp2 && fin>>temp3)
	{
		t.push_back(temp1);
		y.push_back(temp2);
		sig.push_back(temp3);
		inverseVariance.push_back(1./(temp3*temp3));
	}
	cout<<"# Loaded "<<t.size()<<" points from file "<<filename<<"."<<endl;
	fin.close();

	computeSummaries();
	loaded = true;
}

void Data::computeSummaries()
{
	tRange = *max_element(t.begin(), t.end())
			- *min_element(t.begin(), t.end());

	double yTot = 0.;
	double ySqTot = 0.;
	double wTot = 0.;
	for(size_t i=0; i<t.size(); i++)
	{
		yTot +=   inverseVariance[i]*y[i];
		ySqTot += inverseVariance[i]*y[i]*y[i];
		wTot +=   inverseVariance[i];
	}

	yMean = yTot/wTot;
	ySig = sqrt(ySqTot/wTot - pow(yMean, 2));

	cout<<"# ***Data Properties***"<<endl;
	cout<<"# Time range = "<<tRange<<endl;
	cout<<"# Mean of y value = "<<yMean<<endl;
	cout<<"# Stdev y values = "<<ySig<<endl;
}

