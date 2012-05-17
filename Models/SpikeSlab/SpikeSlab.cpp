#include "SpikeSlab.h"

using namespace std;

namespace NewSampler
{

SpikeSlab::SpikeSlab()
:params(20)
{

}

void SpikeSlab::fromPrior()
{

}

double SpikeSlab::perturb()
{
	return 0.;
}

double SpikeSlab::logLikelihood() const
{
	return 0.;
}

void SpikeSlab::print(std::ostream& out) const
{
	out<<' ';
}

} // namespace SpikeSlab

