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

#include "FitSine.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include "Data.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace DNest3;

const int FitSine::maxNumComponents = 10;

FitSine::FitSine()
{

}

void FitSine::fromPrior()
{
	if(!Data::get_instance().get_loaded())
		cerr<<"# Warning: No data loaded!"<<endl;
	mockData.assign(Data::get_instance().get_N(), 0.);

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
	numComponents = randInt(maxNumComponents + 1);

	frequencies.clear();
	amplitudes.clear();
	phases.clear();
	double A, f, phi;
	for(int i=0; i<numComponents; i++)
	{
		A = -muAmplitudes*log(randomU());
		f = exp(minLogFreq + rangeLogFreq*randomU());
		phi = 2*M_PI*randomU();

		addComponent(A, f, phi);
		amplitudes.push_back(A);
		frequencies.push_back(f);
		phases.push_back(phi);
	}
}

double FitSine::perturb()
{
	double logH = 0.;

	if(staleness > 1000)
		calculateMockData();

	return logH;
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
	for(int i=0; i<numComponents; i++)
		addComponent(amplitudes[i], frequencies[i], phases[i]);

	staleness = 0;
}

void FitSine::addComponent(double amplitude, double frequency, double phase)
{
	if(amplitude == 0.)
		return;

	for(size_t i=0; i<mockData.size(); i++)
		mockData[i] += amplitude*sin(2*M_PI*frequency*
				Data::get_instance().get_t(i) + phase);
}

void FitSine::print(std::ostream& out) const
{
	out<<numComponents<<' '<<muAmplitudes<<' '<<staleness<<' ';

	// Print amplitudes, use zero padding
	for(int i=0; i<numComponents; i++)
		out<<amplitudes[i]<<' ';
	for(int i=numComponents; i<maxNumComponents; i++)
		out<<0<<' ';

	// Print frequencies, use zero padding
	for(int i=0; i<numComponents; i++)
		out<<frequencies[i]<<' ';
	for(int i=numComponents; i<maxNumComponents; i++)
		out<<0<<' ';

	// Print phases, use zero padding
	for(int i=0; i<numComponents; i++)
		out<<phases[i]<<' ';
	for(int i=numComponents; i<maxNumComponents; i++)
		out<<0<<' ';
}

string FitSine::description() const
{
	string result("numComponents, muAmplitudes, staleness, ");
	result += "amplitudes, frequencies, phases.";
	return result;
}


