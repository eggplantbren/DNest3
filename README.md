Welcome to DNest3
=================

DNest3 is a reimplementation of Diffusive Nested Sampling, since my C++ skills
have improved. It has improved performance (in terms of the sampling overhead, 
I still expect likelihood evaluations to dominate) and is cleaner code:
implementing new models should be easier than it was before.

It has almost all of the features that the older DNest has. Currently the only
missing feature is multithreading support.

For documentation (currently in a draft state),
please open Docs/index.html in your browser.

The Paper
=========

If you find this software useful,
please cite the following paper:

Diffusive Nested Sampling  
Brendon J. Brewer, Livia B. Pártay, and Gábor Csányi  
Statistics and Computing, 2011, 21, 4, 649-656.  

The paper is [freely available online](http://arxiv.org/abs/0912.2380) at
the [arXiv](http://www.arxiv.org/).

Copyright and Licence
=====================

DNest3 is Copyright (c) 2009-2012 Brendon J. Brewer.

DNest3 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DNest3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DNest3. If not, see <http://www.gnu.org/licenses/>.
