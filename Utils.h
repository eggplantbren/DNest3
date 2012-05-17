/*
    Copyright (C) 2011 Brendon J. Brewer
    This file is part of DNest, the Diffusive Nested Sampler.

    DNest is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DNest is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DNest.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef _Utils_
#define _Utils_

#include <vector>

namespace DNest3
{

extern const double pi;
double mod(double y, double x);
int mod(int y, int x);
double logsumexp(double* logv, int n);
double logsumexp(std::vector<double> logv);
double logsumexp(double a, double b);
int GCD(int a, int b);

} //namespace DNest3

#endif

