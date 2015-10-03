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

#ifndef DNest3_Utils_h
#define DNest3_Utils_h

#include <vector>

namespace DNest3
{

double mod(double y, double x);
int mod(int y, int x);
void wrap(double& x, double min, double max);
double logsumexp(double* logv, int n);
double logsumexp(const std::vector<double>& logv);
double logsumexp(double a, double b);
double logdiffexp(double a, double b);

} //namespace DNest3

#endif

