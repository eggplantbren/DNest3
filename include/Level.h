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

#ifndef DNest3_Level_h
#define DNest3_Level_h

#include "LikelihoodType.h"
#include <vector>
#include <ostream>
#include <istream>

namespace DNest3
{

/*
* This class defines a Nested Sampling level and its
* associated properties.
*/

class Level
{
	private:
		double logX;
		LikelihoodType cutoff;

		long accepts;
		long tries;
		long visits;
		long exceeds;

	public:
		// Constructors
		Level(double logX, const LikelihoodType& cutoff);
		Level(double logX, double logL, double tieBreaker);

		// Getters
		const LikelihoodType& get_cutoff() const { return cutoff; }
		double get_logX() const { return logX; }
		long get_tries() const { return tries; }

		// Update level statistics
		void incrementVisits(bool incrementExceeds);
		void incrementTries(bool accepted);
		void renormaliseVisits(int regularisation);

		// Operations on sets of levels
		static void recalculateLogX(std::vector<Level>& levels, double compression, int regularisation);
		static void renormaliseVisits(std::vector<Level>& levels, int regularisation);
		static std::vector<Level> loadLevels(const char* filename);

		// Combine level statistics
		Level& operator += (const Level& other);
		Level& operator -= (const Level& other);

	// Stream I/O
	friend std::ostream& operator << (std::ostream& out, const Level& level);
	friend std::istream& operator >> (std::istream& in, Level& level);
};

} // namespace DNest3

#endif

