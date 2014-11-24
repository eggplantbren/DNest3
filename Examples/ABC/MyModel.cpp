#include "MyModel.h"
#include "RandomNumberGenerator.h"
#include "Utils.h"
#include "Data.h"
#include <cmath>

using namespace std;
using namespace DNest3;

MyModel::MyModel()
:x_fake_latent(Data::get_instance().get_x().size())
{

}

void MyModel::fromPrior()
{
	mu = -10. + 20.*randomU();
	log_sigma = -10. + 20.*randomU();
	for(size_t i=0; i<x_fake_latent.size(); i++)
		x_fake_latent[i] = randn();
}

double MyModel::perturb()
{
	int which = randInt(3);

	if(which == 0)
	{
		mu += 20.*randh();
		wrap(mu, -10., 10.);
	}
	if(which == 1)
	{
		log_sigma += 20.*randh();
		wrap(log_sigma, -10., 10.);
	}
	if(which == 2)
	{
		int i = randInt(x_fake_latent.size());
		x_fake_latent[i] = randn();
	}

	return 0.;
}

double MyModel::logLikelihood() const
{
	double x_min = Data::get_instance().get_x_min();
	double x_max = Data::get_instance().get_x_max();

	double sigma = exp(log_sigma);

	// Assemble fake dataset
	vector<double> x_fake = x_fake_latent;
	for(size_t i=0; i<x_fake.size(); i++)
		x_fake[i] = mu + sigma*x_fake[i];

	// Goodness
	double logL = 0.;
	logL -= pow(*min_element(x_fake.begin(), x_fake.end()) - x_min, 2);
	logL -= pow(*max_element(x_fake.begin(), x_fake.end()) - x_max, 2);

	return logL;
}

void MyModel::print(std::ostream& out) const
{
	out<<mu<<' '<<log_sigma<<' ';
	for(size_t i=0; i<x_fake_latent.size(); i++)
		out<<x_fake_latent[i]<<' ';
}

string MyModel::description() const
{
	return string("");
}

