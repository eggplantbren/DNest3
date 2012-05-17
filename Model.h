#ifndef _Model_
#define _Model_

#include <vector>
#include <ostream>

namespace NewSampler
{

/* Abstract base class for models */
class Model
{
	protected:
		// Parameters go here

	public:
		// Generate the point from the prior
		virtual void fromPrior() = 0;

		// Metropolis-Hastings proposals
		virtual double perturb() = 0;

		// Likelihood function
		virtual double logLikelihood() const = 0;

		// Print to stream
		virtual void print(std::ostream& out) const = 0;
};

} // namespace NewSampler

#endif

