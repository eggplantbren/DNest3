#include "TestModel.h"

using namespace std;

namespace NewSampler
{

TestModel::TestModel()
:params(20)
{

}

void TestModel::fromPrior()
{

}

double TestModel::perturb()
{
	return 0.;
}

double TestModel::logLikelihood() const
{
	return 0.;
}

void TestModel::print(std::ostream& out) const
{
	out<<' ';
}

} // namespace TestModel

