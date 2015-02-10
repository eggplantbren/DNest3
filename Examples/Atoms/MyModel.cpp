#include "MyModel.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <cmath>

using namespace std;
using namespace DNest3;

MyModel::MyModel()
:N(100)
,x(N), y(N)
,terms(N, vector<double>(N))
,staleness(0)
{

}

void MyModel::fromPrior()
{
	for(size_t i=0; i<x.size(); i++)
	{
		x[i] = randomU();
		y[i] = randomU();
	}

	calculate_terms();
	calculate_PE();
}

void MyModel::calculate_terms()
{
	for(int i=0; i<N; i++)
		for(int j=(i+1); j<N; j++)
			terms[i][j] = calculate_term(i, j);
}

long double MyModel::update_terms(int which)
{
	long double diff = 0.;
	for(int i=0; i<which; i++)
	{
		diff -= terms[i][which];
		terms[i][which] = calculate_term(i, which);
		diff += terms[i][which];
	}
	for(int j=which+1; j<N; j++)
	{
		diff -= terms[which][j];
		terms[which][j] = calculate_term(which, j);
		diff += terms[which][j];
	}
	return diff;
}

double MyModel::calculate_term(int i, int j)
{
	double Rmsq = pow(0.01, 2);
	double rsq = pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2);
	return pow(Rmsq/rsq, 6) - 2.*pow(Rmsq/rsq, 3);
}

void MyModel::calculate_PE()
{
	PE = 0.;
	for(int i=0; i<N; i++)
		for(int j=(i+1); j<N; j++)
			PE += terms[i][j];
	staleness = 0;
}

double MyModel::perturb()
{
	int which = randInt(x.size());

	x[which] += randh();
	y[which] += randh();
	wrap(x[which], 0., 1.);
	wrap(y[which], 0., 1.);

	long double diff = update_terms(which);

	if(fabs(diff) > 1000.*fabs(PE) || staleness >= 20)
		calculate_PE();
	else
	{
		PE += diff;
		staleness++;
	}

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

