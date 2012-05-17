#include <iostream>
#include "Sampler.h"
#include "Models/SpikeSlab/SpikeSlab.h"
#include "RandomNumberGenerator.h"

using namespace std;
using namespace DNest3;

int main()
{
	Sampler<SpikeSlab> sampler;
	cout<<randomU()<<endl;
	return 0;
}

