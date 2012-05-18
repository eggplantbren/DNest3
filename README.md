DNest 3
==========

This is a reimplementation of Diffusive Nested Sampling, since my C++ skills
have improved. It has improved performance (in terms of the sampling overhead, 
I still expect likelihood evaluations to dominate) and is cleaner code:
implementing new models should be even easier.

It has almost all of the features that the older DNest has. Currently missing
features include loading of a levels.txt file, and multithreading support.

(c) 2009-2012 Brendon J. Brewer. Licence: GNU GPL 3.


