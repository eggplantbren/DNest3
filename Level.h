#ifndef _Level_
#define _Level_

#include "LikelihoodType.h"
#include <vector>
#include <ostream>
#include <istream>

namespace DNest3
{

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
		Level(double logX, const LikelihoodType& cutoff);
		Level(double logX, double logL, double tieBreaker);
		LikelihoodType getCutoff() const;
		double getLogX() const;
		int getTries() const;
		void incrementVisits(bool incrementExceeds);
		void incrementTries(bool accepted);
		void renormaliseVisits(int regularisation);

		static void recalculateLogX(std::vector<Level>& levels, int regularisation);
		static void renormaliseVisits(std::vector<Level>& levels, int regularisation);
		static void zero(std::vector<Level>& levels);

	friend std::ostream& operator << (std::ostream& out, const Level& level);
	friend std::istream& operator >> (std::istream& in, Level& level);
};

} // namespace DNest3

#endif

