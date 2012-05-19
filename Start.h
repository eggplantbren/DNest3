/*
* Copyright (c) 2009, 2010, 2011, 2012 Brendon J. Brewer.
*
* This file is part of DNest.
*
* DNest is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DNest is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DNest. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _Start_
#define _Start_

namespace DNest3
{

/*
* A little function that does everything
* needed to start DNest3 running. Keeps main() clean.
*/
template<class ModelType>
void start(int argc, char** argv);

} // namespace DNest3

#include "StartImpl.h"

#endif

