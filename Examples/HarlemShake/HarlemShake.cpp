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

#include "HarlemShake.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <cmath>
#include <fstream>

using namespace std;
using namespace DNest3;

HarlemShake::HarlemShake()
{

}

void HarlemShake::fromPrior()
{
	// Uniform prior in [0, 1]^2
	x = randomU();
	y = randomU();
}

void HarlemShake::density()
{
	fstream fout("shake.txt", ios::out);
	HarlemShake shake;
	for(shake.y = 1. - 0.5*0.01; shake.y >= 0.; shake.y -= 0.01)
	{
		for(shake.x = 0. + 0.5*0.01; shake.x <= 1.; shake.x += 0.01)
			fout<<shake.logLikelihood()<<' ';
		fout<<endl;
	}
	fout.close();
}

double HarlemShake::perturb()
{
	double scale = pow(10., 1.5 - 4.*randomU());
	x += scale*randn();
	y += scale*randn();

	double logH = 0.;
	if(x < 0 || x > 1 || y < 0 || y > 1)
		logH = -1E300;

	return logH;
}

#include <iostream>

double HarlemShake::logLikelihood() const
{
	double C = 0.333333333;

	// Locate the point in one of the nine boxes
	int xBox = (int)(x/C);
	int yBox = (int)(y/C);

	double logL = 0.;

	// Bottom row
	if(xBox == 0 && yBox == 0)
		logL = S(x/C, y/C);
	if(xBox == 1 && yBox == 0)
		logL = H((x - C)/C, y/C);
	if(xBox == 2 && yBox == 0)
		logL = K((x - 2*C)/C, y/C);

	// Middle row
	if(xBox == 0 && yBox == 1)
		logL = L(x/C, (y - C)/C);
	if(xBox == 1 && yBox == 1)
		logL = O((x - C)/C, (y - C)/C);
	if(xBox == 2 && yBox == 1)
		logL = M((x - 2*C)/C, (y - C)/C);

	// Top row
	if(xBox == 0 && yBox == 2)
		logL = H(x/C, (y - 2*C)/C);
	if(xBox == 1 && yBox == 2)
		logL = A((x - C)/C, (y - 2*C)/C);
	if(xBox == 2 && yBox == 2)
		logL = R((x - 2*C)/C, (y - 2*C)/C);

	return logL;
}

void HarlemShake::print(std::ostream& out) const
{
	out<<x<<' '<<y;
}

string HarlemShake::description() const
{
	return string("x, y");
}

// The letters
double HarlemShake::H(double x, double y)
{
	double result = -1E300;
	result = logsumexp(result, gaussian(x, y, 0.1, 0.5, 0.3, 0.1, 0.5*M_PI));
	result = logsumexp(result, gaussian(x, y, 0.9, 0.5, 0.3, 0.1, 0.5*M_PI));
	result = logsumexp(result, gaussian(x, y, 0.5, 0.5, 0.3, 0.1, 0.));
	return result;
}

double HarlemShake::A(double x, double y)
{
	double result = -1E300;
	result = logsumexp(result, gaussian(x, y, 0.3, 0.5, 0.3, 0.1,  0.35*M_PI));
	result = logsumexp(result, gaussian(x, y, 0.5, 0.5, 0.3, 0.1,         0.));
	result = logsumexp(result, gaussian(x, y, 0.7, 0.5, 0.3, 0.1, -0.35*M_PI));
	return result;
}

double HarlemShake::R(double x, double y)
{
	double result = -1E300;
	result = logsumexp(result, gaussian(x, y, 0.1, 0.5, 0.3, 0.1, 0.5*M_PI));
	result = logsumexp(result, gaussian(x, y, 0.5, 0.75, 0.3, 0.1, -0.1*M_PI));
	result = logsumexp(result, gaussian(x, y, 0.5, 0.5,  0.3, 0.1,  0.1*M_PI));
	result = logsumexp(result, gaussian(x, y, 0.5, 0.3,  0.3, 0.1, -0.15*M_PI));
	return result;
}

double HarlemShake::L(double x, double y)
{
	double result = -1E300;
	result = logsumexp(result, gaussian(x, y, 0.1, 0.5, 0.3, 0.1, 0.5*M_PI));
	result = logsumexp(result, gaussian(x, y, 0.5, 0.1, 0.3, 0.1, 0.));
	return result;
}

double HarlemShake::O(double x, double y)
{
	return 1. + gaussian(x, y, 0.5, 0.5, 0.3, 1., 0.);
}

double HarlemShake::M(double x, double y)
{
	double result = -1E300;
	result = logsumexp(result, gaussian(x, y, 0.1, 0.5, 0.3, 0.1, 0.5*M_PI));
	result = logsumexp(result, gaussian(x, y, 0.9, 0.5, 0.3, 0.1, 0.5*M_PI));
	result = logsumexp(result, gaussian(x, y, 0.25, 0.6, 0.3, 0.1, -0.3*M_PI));
	result = logsumexp(result, gaussian(x, y, 0.75, 0.6, 0.3, 0.1,  0.3*M_PI));
	return result;
}

double HarlemShake::S(double x, double y)
{
	double result1 = -0.5*pow(pow(x - 0.5, 2) + pow(y - 0.75, 2) - pow(0.2, 2), 2)/pow(0.02, 2);
	if(x > 0.5 && y > 0.5 && y < 0.8)
		result1 = -1E300;
	double result2 = -0.5*pow(pow(x - 0.5, 2) + pow(y - 0.25, 2) - pow(0.2, 2), 2)/pow(0.02, 2);
	if(x < 0.5 && y > 0.2 && y < 0.5)
		result2 = -1E300;
	return 3. + logsumexp(result1, result2);
}

double HarlemShake::K(double x, double y)
{
	double result = -1E300;
	result = logsumexp(result, gaussian(x, y, 0.1, 0.5, 0.3, 0.1, 0.5*M_PI));
	result = logsumexp(result, gaussian(x, y, 0.4, 0.7, 0.3, 0.1,  0.25*M_PI));
	result = logsumexp(result, gaussian(x, y, 0.4, 0.3, 0.3, 0.1, -0.25*M_PI));
	return result;
}

double HarlemShake::gaussian(double x, double y, double xc, double yc, double w, double q, double theta)
{
	double xx =  (x - xc)*cos(theta) + (y - yc)*sin(theta);
	double yy = -(x - xc)*sin(theta) + (y - yc)*cos(theta);
	double rsq = pow(xx, 2) + pow(yy/q, 2);
	double ww = pow(w, 2);
	return -log(2.*M_PI*ww*q) - 0.5*rsq/ww;
}

