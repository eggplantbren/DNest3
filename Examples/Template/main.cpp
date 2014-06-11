#include <iostream>
#include "Start.h"
#include "MyModel.h"

using namespace std;
using namespace DNest3;

int main(int argc, char** argv)
{
	#ifndef DNest3_No_Boost
	MTSampler<MyModel> sampler = setup_mt<MyModel>(argc, argv);
	#else
	Sampler<MyModel> sampler = setup<MyModel>(argc, argv);
	#endif

	sampler.run();
	return 0;
}

