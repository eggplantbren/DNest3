#include "MyModel.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <cmath>

using namespace std;
using namespace DNest3;

MyModel::MyModel()
{

}

void MyModel::fromPrior()
{

}

double MyModel::perturb()
{
	return 0.;
}

double MyModel::logLikelihood() const
{
	return 0.;
}

void MyModel::print(std::ostream& out) const
{

}

string MyModel::description() const
{
	return string("");
}

