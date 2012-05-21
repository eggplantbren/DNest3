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

#include "LikelihoodType.h"

namespace DNest3
{

LikelihoodType::LikelihoodType()
{
}

LikelihoodType::LikelihoodType(double logL, double tieBreaker)
:logL(logL)
,tieBreaker(tieBreaker)
{
}

bool operator < (const LikelihoodType& l1, const LikelihoodType& l2)
{
	bool result = false;
	if(l1.logL < l2.logL)
		result = true;
	else if(l1.logL == l2.logL && l1.tieBreaker < l2.tieBreaker)
		result = true;
	return result;
}

} // namespace DNest3

