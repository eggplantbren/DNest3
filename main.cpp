#include <iostream>
#include "Sampler.h"
#include "Models/SpikeSlab/SpikeSlab.h"
#include "RandomNumberGenerator.h"

using namespace std;
using namespace DNest3;

int main()
{
	Options options("OPTIONS");
	Sampler<SpikeSlab> sampler(options);
	cout<<randomU()<<endl;
	return 0;
}

