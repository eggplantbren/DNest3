/*
* Copyright (c) 2009, 2010, 2011, 2012 Brendon J. Brewer.
*
* This file is part of DNest3.
*
* DNest3 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DNest3 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DNest3. If not, see <http://www.gnu.org/licenses/>.
*/

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
		int get_N() { return static_cast<int>(y.size()); }

	// Static stuff for singleton use
	private:
		static Data instance;

	public:
		// Access to the instance
		static Data& get_instance() { return instance; }
};

#endif

