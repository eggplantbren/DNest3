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

#ifndef DNest3_Start_h
#define DNest3_Start_h

#include "CommandLineOptions.h"
#include "Sampler.h"

namespace DNest3
{

/*
* Set up the sampler and return it
* You'll still need to call run() on it
*/
template<class ModelType>
Sampler<ModelType> setup(int argc, char** argv);

template<class ModelType>
Sampler<ModelType> setup(const CommandLineOptions& options);

/*
* A little function that does everything
* needed to start DNest3 running. Keeps main() clean.
*/
template<class ModelType>
void start(int argc, char** argv);

} // namespace DNest3

#include "StartImpl.h"

#endif

