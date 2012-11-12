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

#include "Polynomial.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include "Data.h"
#include <cmath>

using namespace std;
using namespace DNest3;

Polynomial::Polynomial()
:a(5)
{

}

void Polynomial::fromPrior()
{
	for(size_t i=0; i<a.size(); i++)
		a[i] = randn();
}

double Polynomial::perturb()
{
	double logH = 0.;
	int which = randInt(a.size());

	logH -= -0.5*pow(a[which], 2);
	a[which] += pow(10., 1.5 - 6.*randomU())*randn();
	logH += -0.5*pow(a[which], 2);

	return logH;
}

double Polynomial::logLikelihood() const
{
	double logL = 0.;
	vector<double> mock_data(Data::get_instance().get_N(), 0.);

	for(size_t i=0; i<mock_data.size(); i++)
	{
		for(size_t j=0; j<a.size(); j++)
			mock_data[i] += a[j]*pow(Data::get_instance().get_x(i), j);
	}

	for(size_t i=0; i<mock_data.size(); i++)
		logL += -0.5*pow((Data::get_instance().get_y(i) - mock_data[i])/0.2, 2);

	return logL;
}

void Polynomial::print(std::ostream& out) const
{
	for(size_t i=0; i<a.size(); i++)
		out<<a[i]<<' ';
}

string Polynomial::description() const
{
	return string("Each column is one of the 5 coefficients.");
}

