Welcome to DNest3
=================

DNest3 is a C++ implementation of Diffusive Nested Sampling, a Markov Chain
Monte Carlo (MCMC) algorithm for Bayesian Inference and Statistical Mechanics.

Relative to older DNest versions, DNest3 has improved performance
(in terms of the sampling overhead, likelihood evaluations still dominate in
general) and is cleaner code: implementing new models should be easier than it
was before.

For documentation (currently in a draft state),
please open Docs/index.html in your browser or
click [here](http://eggplantbren.github.com/DNest3).

Dependencies
============

DNest3 requires that you have the following packages installed on your
computer:

* [GNU Scientific Library (GSL)](http://www.gnu.org/software/gsl/)
* [Boost](http://www.boost.org/) (optional but highly recommended)
* [Python 2](http://www.python.org/)
* [NumPy](http://numpy.scipy.org/)
* [matplotlib](http://matplotlib.sourceforge.net/)

Building
========

You can build DNest3 using [CMake](http://www.cmake.org/).

Mac
---

If you're using a Mac, hopefully you're also using
[Homebrew](http://mxcl.github.com/homebrew/). Please do it.

Now that we've got that out of the way, install the dependencies:


```
brew install cmake gsl
```

and optionally:

```
brew install boost
```

Then build DNest3:

```
git clone https://github.com/eggplantbren/DNest3.git
cd DNest3
mkdir build
cd build
cmake ..
make
make install
```

Ubuntu
------

First, install the dependencies

```
sudo apt-get install cmake libgsl0-dev
```

and optionally

```
sudo apt-get install libboost-all-dev
```

Then build the library:

```
git clone https://github.com/eggplantbren/DNest3.git
cd DNest3
mkdir build
cd build
cmake ..
make
sudo make install
```


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

Contributors
============

Daniel Foreman-Mackey (NYU)

