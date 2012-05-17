#ifndef _Level_
#define _Level_

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

		int accepts;
		int tries;
		int visits;
		int exceeds;

	public:
		// Constructors
		Level(double logX, const LikelihoodType& cutoff);
		Level(double logX, double logL, double tieBreaker);

		// Getters
		const LikelihoodType& get_cutoff() const { return cutoff; }
		double get_logX() const { return logX; }

		// Update level statistics
		void incrementVisits(bool incrementExceeds);
		void incrementTries(bool accepted);
		void renormaliseVisits(int regularisation);

		// Operations on sets of levels
		static void recalculateLogX(std::vector<Level>& levels, int regularisation);
		static void renormaliseVisits(std::vector<Level>& levels, int regularisation);

	// Stream I/O
	friend std::ostream& operator << (std::ostream& out, const Level& level);
	friend std::istream& operator >> (std::istream& in, Level& level);
};

} // namespace DNest3

#endif

