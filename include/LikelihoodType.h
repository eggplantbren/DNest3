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

#ifndef DNest3_LikelihoodType_h
#define DNest3_LikelihoodType_h

#include <istream>
#include <ostream>

namespace DNest3
{

// Forward declaration
class Level;

class LikelihoodType
{
	private:
		double logL;
		double tieBreaker;

	public:
		LikelihoodType();
		LikelihoodType(double logL, double tieBreaker);

	// Various stuff needs to be a friend
	friend class Level;
	template<class ModelType> friend class Sampler;
	template<class ModelType> friend class MTSampler;

	friend bool operator < (const LikelihoodType& l1, const LikelihoodType& l2);
	friend std::ostream& operator << (std::ostream& out, const Level& level);
	friend std::istream& operator >> (std::istream& in, Level& level);
};

bool operator < (const LikelihoodType& l1, const LikelihoodType& l2);

} // namespace DNest3

#endif

