import DNest3
import MyModel

#Load options from file...    
options=DNest3.Options(
    1,
    10000,
    100000,
    200,
    100,
    10,
    10,
    0
)

#Set compression...
compression = 3.

DNest3.initialise_rng()

sampler = DNest3.Sampler(MyModel.model.model,compression,options)
sampler.run()
