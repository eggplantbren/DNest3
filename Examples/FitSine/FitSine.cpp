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

#include "FitSine.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include "Data.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace DNest3;

const int FitSine::maxNumFrequencies = 100;

FitSine::FitSine()
:u_amplitudes(maxNumFrequencies)
,frequencies(maxNumFrequencies)
,phases(maxNumFrequencies)
{

}

void FitSine::fromPrior()
{
	if(!Data::get_instance().get_loaded())
		cerr<<"# Warning: No data loaded!"<<endl;

	// Set limits on muAmplitudes
	minLogMu = log(1E-3*Data::get_instance().get_ySig());
	maxLogMu = minLogMu + log(1E4);
	rangeLogMu = maxLogMu - minLogMu;

	// Set limits on frequencies
	// Longest period = 10*data range
	// Shortest period = 1E-4*longest period
	minLogFreq = log(1./(10.*Data::get_instance().get_tRange()));
	maxLogFreq = log(1E4) + minLogFreq;
	rangeLogFreq = maxLogFreq - minLogFreq;

	muAmplitudes = exp(minLogMu + rangeLogMu*randomU());
	for(int i=0; i<maxNumFrequencies; i++)
	{
		u_amplitudes[i] = randomU();
		frequencies[i] = exp(minLogFreq + rangeLogFreq*randomU());
		phases[i] = 2*M_PI*randomU();
	}
}

double FitSine::perturb()
{
	int which = randInt(1);
	if(which == 0)
	{
		muAmplitudes = log(muAmplitudes);
		muAmplitudes += rangeLogMu*pow(10., 1.5 - 6.*randomU())*randn();
		muAmplitudes = mod(muAmplitudes - minLogMu,
				rangeLogMu) + minLogMu;
		muAmplitudes = exp(muAmplitudes);
	}
	return 0.;
}

double FitSine::logLikelihood() const
{
	return 0.;
}

void FitSine::print(std::ostream& out) const
{
	out<<muAmplitudes<<' ';
}

