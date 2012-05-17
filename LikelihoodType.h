#ifndef _LikelihoodType_
#define _LikelihoodType_

namespace NewSampler
{

struct LikelihoodType
{
	double logL;
	double tieBreaker;
};

bool operator < (const LikelihoodType& l1, const LikelihoodType& l2);

} // namespace NewSampler

#endif

