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

#include "Gauss.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <cmath>

using namespace std;
using namespace DNest3;

Gauss::Gauss()
{

}

void Gauss::fromPrior()
{
	u = randomU();
}

double Gauss::perturb()
{
	//cout << "perturbing from " << u << " to ";
	u += pow(10., 1.5-6*randomU())*randn();
	//u += randn() / 100;
	u = mod(u + 100, 1.);
	//cout << u << endl;
	return 0.;
}

double Gauss::logLikelihood() const
{
	return -log(0.01 * sqrt(2*M_PI)) - 0.5 * pow((u - 0.654321) / 0.01, 2);
}

void Gauss::print(std::ostream& out) const
{
	out << u << " 1 ";
}

string Gauss::description() const
{
	return string("Gauss");
}

