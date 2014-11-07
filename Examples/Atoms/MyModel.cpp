#include "MyModel.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <cmath>

using namespace std;
using namespace DNest3;

MyModel::MyModel()
:x(200), y(200)
{

}

void MyModel::fromPrior()
{
	for(size_t i=0; i<x.size(); i++)
	{
		x[i] = randomU();
		y[i] = randomU();
	}

	calculate_PE();
}

void MyModel::calculate_PE()
{
	PE = 0.;
	for(size_t i=0; i<x.size(); i++)
		for(size_t j=(i+1); j<x.size(); j++)
			PE += calculate_PE(i, j);
}

double MyModel::calculate_PE(int i, int j)
{
	double Rmsq = pow(0.01, 2);
	double rsq = pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2);
	return pow(Rmsq/rsq, 6) - 2.*pow(Rmsq/rsq, 3);
}

double MyModel::perturb()
{
	int which = randInt(x.size());

	double diff = 0.;
	for(size_t i=0; i<x.size(); i++)
		if((int)i != which)
			diff -= calculate_PE(i, which);

	x[which] += randh();
	y[which] += randh();
	wrap(x[which], 0., 1.);
	wrap(y[which], 0., 1.);

	for(size_t i=0; i<x.size(); i++)
		if((int)i != which)
			diff += calculate_PE(i, which);

	// If fractional change is big
	if(abs(diff)/abs(PE) > 1. || randomU() < 0.01)
		calculate_PE();
	else
		PE += diff;

	return 0.;
}

double MyModel::logLikelihood() const
{
	return -PE;
}

void MyModel::print(std::ostream& out) const
{
	for(size_t i=0; i<x.size(); i++)
		out<<x[i]<<' ';
	for(size_t i=0; i<x.size(); i++)
		out<<y[i]<<' ';
}

string MyModel::description() const
{
	return string("");
}

