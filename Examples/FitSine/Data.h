#ifndef _Data_
#define _Data_

#include <vector>

/*
* A dataset to fit sine waves to.
* Also contains a static singleton for the one-data-set case
*/

class Data
{
	private:
		// A flag
		bool loaded;

		// The data
		std::vector<double> t, y, sig, inverseVariance;

		// Summary statistics
		double tRange, yMean, ySig;

		// Compute the summary statistics from the data
		void computeSummaries();

	public:
		// Default constructor: Empty data
		Data();

		// Load the data from file
		void load(const char* filename);

		// Getters
		bool get_loaded() { return loaded; }
		double get_t(int i) { return t[i]; }
		double get_y(int i) { return y[i]; }
		double get_sig(int i) { return sig[i]; }
		double get_tRange() { return tRange; }
		double get_yMean() { return yMean; }
		double get_ySig() { return ySig; }

	// Static stuff for singleton use
	private:
		static Data instance;

	public:
		// Access to the instance
		static Data& get_instance() { return instance; }
};

#endif

