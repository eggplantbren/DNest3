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

		// Prior density
		virtual double logPrior() const = 0;

		// Likelihood function
		virtual double logLikelihood() const = 0;

		// Pack the parameters into a vector
		virtual std::vector<double> toVector() const = 0;

		// Unpack the parameters from a vector
		virtual void fromVector(const std::vector<double>& params) = 0;

		// Print to stream
		virtual void print(std::ostream& out) const;
};

} // namespace NewSampler

#endif

