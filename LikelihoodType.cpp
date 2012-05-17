#include "LikelihoodType.h"

namespace DNest3
{

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

