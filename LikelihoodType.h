#ifndef _LikelihoodType_
#define _LikelihoodType_

namespace DNest3
{

class LikelihoodType
{
	public:
		double logL;
		double tieBreaker;

		LikelihoodType();
		LikelihoodType(double logL, double tieBreaker);
};

bool operator < (const LikelihoodType& l1, const LikelihoodType& l2);

} // namespace DNest3

#endif

