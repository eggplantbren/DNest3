#ifndef _SpikeSlab_
#define _SpikeSlab_

#include "Model.h"
#include <vector>

class SpikeSlab:public DNest3::Model
{
	private:
		std::vector<double> params;

	public:
		SpikeSlab();

		// Generate the point from the prior
		void fromPrior();

		// Metropolis-Hastings proposals
		double perturb();

		// Likelihood function
		double logLikelihood() const;

		// Print to stream
		void print(std::ostream& out) const;


};

#endif

