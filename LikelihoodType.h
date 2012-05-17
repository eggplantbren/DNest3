#ifndef _LikelihoodType_
#define _LikelihoodType_

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
	friend bool operator < (const LikelihoodType& l1, const LikelihoodType& l2);
	friend std::ostream& operator << (std::ostream& out, const Level& level);
	friend std::istream& operator >> (std::istream& in, Level& level);
};

bool operator < (const LikelihoodType& l1, const LikelihoodType& l2);

} // namespace DNest3

#endif

