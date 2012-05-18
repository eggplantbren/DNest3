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
	mockData.resize(Data::get_instance().get_N());

	onFraction = exp(log(1E-3) + log(1E3)*randomU());

	// Set limits on muAmplitudes
	minLogMu = log(1E-3*Data::get_instance().get_ySig());
	maxLogMu = minLogMu + log(1E4);
	rangeLogMu = maxLogMu - minLogMu;

	// Set limits on frequencies
	// Longest possible period = 10*data range
	// Shortest possible period = 1E-4*longest period
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
	int which = randInt(4);
	if(which == 0)
	{
		onFraction = log(onFraction);
		onFraction += log(1E3)*pow(10., 1.5 - 6.*randomU())*randn();
		onFraction = mod(onFraction - log(1E-3), log(1E3)) + log(1E-3);
		onFraction = exp(onFraction);
	}
	else if(which == 1)
	{
		muAmplitudes = log(muAmplitudes);
		muAmplitudes += rangeLogMu*pow(10., 1.5 - 6.*randomU())*randn();
		muAmplitudes = mod(muAmplitudes - minLogMu,
				rangeLogMu) + minLogMu;
		muAmplitudes = exp(muAmplitudes);
	}
	else if(which == 2)
	{

	}
	else if(which == 3)
	{

	}
	else if(which == 4)
	{

	}

	calculateMockData();
	return 0.;
}

double FitSine::logLikelihood() const
{
	double logL = -0.5*mockData.size()*log(2*M_PI);
	for(size_t i=0; i<mockData.size(); i++)
	{
		double y = Data::get_instance().get_y(i);
		double sig = Data::get_instance().get_sig(i);
		logL += -log(sig);
		logL += -0.5*pow((y - mockData[i])/sig, 2);
	}
	return logL;
}

void FitSine::calculateMockData()
{
	// Zero the mock data
	for(size_t i=0; i<mockData.size(); i++)
		mockData[i] = 0.;

	// Add each frequency
	for(int i=0; i<maxNumFrequencies; i++)
		addFrequency(transform(u_amplitudes[i]), frequencies[i], phases[i]);
}

void FitSine::addFrequency(double amplitude, double frequency, double phase)
{
	if(amplitude == 0.)
		return;

	for(size_t i=0; i<mockData.size(); i++)
		mockData[i] += amplitude*sin(2*M_PI*frequency*
				Data::get_instance().get_t(i) + phase);
}

void FitSine::print(std::ostream& out) const
{
	out<<onFraction<<' '<<muAmplitudes<<' ';
	for(int i=0; i<maxNumFrequencies; i++)
		out<<transform(u_amplitudes[i])<<' ';
	for(int i=0; i<maxNumFrequencies; i++)
		out<<frequencies[i]<<' ';
	for(int i=0; i<maxNumFrequencies; i++)
		out<<phases[i]<<' ';
}

double FitSine::transform(double u_amplitude) const
{
	double A; // Amplitude to return
	double t = 1. - onFraction;

	// Compute flux
	if(u_amplitude < t)
		A = 0.;
	else
	{
		double U = (u_amplitude - t)/onFraction; // U(0, 1)
		A = -muAmplitudes*log(1. - U); // Must use CDF not 1-CDF
	}
	return A;
}

