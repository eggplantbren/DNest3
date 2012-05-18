DNest 3
==========

This is a reimplementation of Diffusive Nested Sampling, since my C++ skills
have improved. It has improved performance (in terms of the sampling overhead, 
I still expect likelihood evaluations to dominate) and is cleaner code:
implementing new models should be even easier.

It has almost all of the features that the older DNest has. Currently the only
missing feature is multithreading support.

(c) 2009-2012 Brendon J. Brewer.

DNest is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DNest is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DNest. If not, see <http://www.gnu.org/licenses/>.
