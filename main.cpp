#include <iostream>
#include <ctime>
#include "Sampler.h"
#include "Models/SpikeSlab/SpikeSlab.h"
#include "RandomNumberGenerator.h"

using namespace std;
using namespace DNest3;

int main()
{
	Options options("OPTIONS");
	RandomNumberGenerator::get_instance().setSeed(-time(0));
	Sampler<SpikeSlab> sampler(options);
	sampler.run();
	return 0;
}

