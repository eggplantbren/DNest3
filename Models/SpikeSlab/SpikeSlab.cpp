#include "SpikeSlab.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <cmath>

using namespace std;
using namespace DNest3;

SpikeSlab::SpikeSlab()
:params(20)
{

}

void SpikeSlab::fromPrior()
{
	for(size_t i=0; i<params.size(); i++)
		params[i] = -0.5 + randomU();
}

double SpikeSlab::perturb()
{
	int which = randInt(params.size());
	params[which] += pow(10., 1.5-6*randomU())*randn();
	params[which] = mod(params[which] + 0.5, 1.) - 0.5;
	return 0.;
}

double SpikeSlab::logLikelihood() const
{
	double u = 0.01;
	double v = 0.1;
	double C = log(1.0/sqrt(2*pi));
	double logl1 = 0;
	double logl2 = 0;

	for(size_t i=0; i<params.size(); i++)
	{
		logl1 += C - log(u) - 0.5*pow((params[i] - 0.031)/u, 2);
		logl2 += C - log(v) - 0.5*pow(params[i]/v, 2);
	}
	logl1 += log(100.0);

	return logsumexp(logl1, logl2);
}

void SpikeSlab::print(std::ostream& out) const
{
	for(size_t i=0; i<params.size(); i++)
		out<<params[i]<<' ';
}

