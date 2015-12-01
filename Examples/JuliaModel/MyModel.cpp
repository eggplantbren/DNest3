#include "MyModel.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include <cmath>
#include <julia/julia.h>

using namespace std;
using namespace DNest3;

MyModel::MyModel()
:x(20)
{

}

void MyModel::fromPrior()
{
	for(size_t i=0; i<x.size(); i++)
		x[i] = randomU();
}

double MyModel::perturb()
{
	int which = randInt(x.size());
	x[which] += randh();
	wrap(x[which], 0., 1.);
	return 0.;
}

double MyModel::logLikelihood() const
{
	jl_value_t* array_type = jl_apply_array_type(jl_float64_type, 1);

	double* temp = const_cast<double*>(&x[0]);
	jl_array_t* params = jl_ptr_to_array_1d(array_type, temp, x.size(), 0);
	jl_function_t* func = jl_get_function(jl_current_module, "log_likelihood");
	jl_value_t* logL = jl_call1(func, reinterpret_cast<jl_value_t*>(params));

	return jl_unbox_float64(logL);
}

void MyModel::print(std::ostream& out) const
{
	for(size_t i=0; i<x.size(); i++)
		out<<x[i]<<' ';
}

string MyModel::description() const
{
	return string("");
}

