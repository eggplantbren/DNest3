#include <iostream>
#include "Start.h"
#include "MyModel.h"
#include <julia/julia.h>

using namespace std;
using namespace DNest3;

int main(int argc, char** argv)
{
	jl_init_with_image("/usr/local/lib/julia/", "sys.so");
	jl_eval_string("include(\"julia_model.jl\")");

	MTSampler<MyModel> sampler = setup_mt<MyModel>(argc, argv);
	sampler.run();

	jl_atexit_hook(0);
	return 0;
}

