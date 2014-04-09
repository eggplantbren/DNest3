import os
import sys
import weakref

from ctypes import *
import abc
from abc import ABCMeta

#Get handle to C API
lib = cdll.LoadLibrary(os.path.join('/home/ben/reps/DNest3/lib','libdnest3.so'))

#Options
lib.Options_new.restype = c_void_p
lib.Options_new.argtypes = [c_int,c_int,c_int,c_int,c_int,c_double,c_double,c_int]
lib.Options_delete.restype = None
lib.Options_delete.argtypes = [c_void_p]

class Options(object):
    def __init__(
        self,
        numParticles,
        newLevelInterval,
        saveInterval,
        threadSteps,
        maxNumLevels,
        Lambda,
        beta,
        maxNumSamples
    ):
        
        self.options=lib.Options_new(
            numParticles,
            newLevelInterval,
            saveInterval,
            threadSteps,
            maxNumLevels,
            Lambda,
            beta,
            maxNumSamples,
        )

    def __del__(self):
        lib.Options_delete(self.options)
        
#LikelihoodType

lib.LikelihoodType_new1.restype = c_void_p
lib.LikelihoodType_new1.argtypes = []
lib.LikelihoodType_new2.restype = c_void_p
lib.LikelihoodType_new2.argtypes = [c_double,c_double]
lib.LikelihoodType_delete.restype = None
lib.LikelihoodType_delete.argtypes = [c_void_p]
lib.LikelihoodType_lt.restype = None
lib.LikelihoodType_lt.argtypes = [c_void_p,c_void_p]

class LikelihoodType(object):
    def __init__(self,*args):
        if not args:
            self.lt=lib.LikelihoodType_new1()
        else:
            self.lt=lib.LikelihoodType_new2(*args)

    def __del__(self):
        lib.LikelihoodType.LikelihoodType_delete(self.lt)

    def __lt__(self,other):
        if isinstance(other,LikelihoodType):
            lib.LikelihoodType_lt(self.lt,other.lt)
        
#Level

lib.Level_new1.restype = c_void_p
lib.Level_new1.argtypes = [c_double,c_void_p]
lib.Level_new2.restype = c_void_p
lib.Level_new2.argtypes = [c_double,c_double,c_double]
lib.Level_delete.restype = None
lib.Level_delete.argtypes = [c_void_p]
lib.Level_get_cutoff.restype = None
lib.Level_get_cutoff.argtypes = [c_void_p]
lib.Level_get_logX.restype = None
lib.Level_get_logX.argtypes = [c_void_p]
lib.Level_get_tries.restype = None
lib.Level_get_tries.argtypes = [c_void_p]
lib.Level_incrementVisits.restype = None
lib.Level_incrementVisits.argtypes = [c_void_p,c_bool]
lib.Level_incrementTries.restype = None
lib.Level_incrementTries.argtypes = [c_void_p,c_bool]
lib.Level_renormaliseVisits1.restype = None
lib.Level_renormaliseVisits1.argtypes = [c_void_p,c_int]
lib.Level_renormaliseVisits2.restype = None
lib.Level_renormaliseVisits2.argtypes = [c_void_p,c_void_p,c_int]
lib.Level_recalculateLogX.restype = None
lib.Level_recalculateLogX.argtypes = [c_void_p,c_void_p,c_double,c_int]
lib.Level_plusequals.restype = c_void_p
lib.Level_plusequals.argtypes = [c_void_p,c_void_p]
lib.Level_minusequals.restype = c_void_p
lib.Level_minusequals.argtypes = [c_void_p,c_void_p]

class Level(object):
    def __init__(self,*args):
        if len(args) is 2:
            self.level=lib.Level_new1(*args)
        elif len(args) is 3:
            self.level=lib.Level_new2(*args)
        else:
            raise RuntimeError
        
    def __del__(self):
        lib.Level_delete(self.level)

    def get_cutoff(self):
        lib.Level_getcutoff(self.level)

    def get_logX(self):
        lib.Level_get_logX(self.level)

    def get_tries(self):
        lib.Level_get_tries(self.level)

    def incrementVisits(self,incrementExceeds):
        lib.Level_incrementVisits(self.level,incrementExceeds)

    def incrementTries(self,accepted):
        lib.Level_incrementTries(self.level,accepted)

    def renormaliseVisits(self,regularisation):
        lib.Level_renormaliseVisits(self.level,regularisation)

    def recalculateLogX(self,levels,compression,regularisation):
        lib.Level_recalculateLogX(self.level,levels,compression,regularisation)

    def loadLevels(self,filename):
        lib.Level_loadLevels(self.level,filename)

#Levels
#Adapted from http://stackoverflow.com/questions/16885344/how-to-handle-c-return-type-stdvectorint-in-python-ctypes .
 
lib.Levels_new.restype = c_void_p
lib.Levels_new.argtypes = []
lib.Levels_delete.restype = None
lib.Levels_delete.argtypes = [c_void_p]
lib.Levels_size.restype = c_int
lib.Levels_size.argtypes = [c_void_p]
lib.Levels_get.restype = c_int
lib.Levels_get.argtypes = [c_void_p, c_int]
lib.Levels_push_back.restype = None
lib.Levels_push_back.argtypes = [c_void_p, c_void_p]

class Levels(list):

    def __init__(self):
        self.levels = lib.Levels_new()  # pointer to new levels

    def __del__(self):  # when reference count hits 0 in Python,
        lib.Levels_delete(self.levels)  # call C++ levels destructor

    def __len__(self):
        return Levels_size(self.levels)

    def __getitem__(self, i):  # access elements in levels at index
        if 0 <= i < len(self):
            return Levels_get(self.levels, c_int(i))
        raise IndexError('Levels index out of range')

    def __repr__(self):
        return '[{}]'.format(', '.join(str(self[i]) for i in range(len(self))))

    def push(self, i):  # push calls levels's push_back
        Levels_push_back(self.levels, c_int(i))

#RandomNumberGenerator
lib.RandomNumberGenerator_initialise_instance.restype = None
lib.RandomNumberGenerator_initialise_instance.argtypes = []
lib.RandomNumberGenerator_set_seed.restypes = None
lib.RandomNumberGenerator_set_seed.argtypes = [c_long]

#CModel
class CModel(object):
    def __init__(
        self,
        cbfuncs,
        state,
        weakrefs,
    ):
        self.model=lib.CModel_new(
            cbfuncs,
            state,
        )

        self.__weakrefs=weakrefs
      
CModel_fromPrior_cb_t=CFUNCTYPE(None,c_void_p)
CModel_perturb_cb_t=CFUNCTYPE(c_double,c_void_p)
CModel_logLikelihood_cb_t=CFUNCTYPE(c_double,c_void_p)
CModel_print_sample_cb_t=CFUNCTYPE(None,c_void_p,c_void_p)
CModel_copy_cb_t=CFUNCTYPE(None,c_void_p,c_void_p)
CModel_del_cb_t=CFUNCTYPE(None,c_void_p)
CModel_description_cb_t=CFUNCTYPE(c_char_p,c_void_p)

class CModelMethods(Structure):
    pass

lib.CModel_delete.restype=None
lib.CModel_delete.argtypes=[c_void_p]

lib.CModel_new.restype= c_void_p
lib.CModel_new.argtypes= [
       POINTER(CModelMethods),
       c_void_p,
]
    
CModelMethods._fields_= [
       ('fromPrior',CModel_fromPrior_cb_t),
       ('perturb',CModel_perturb_cb_t),
       ('logLikelihood',CModel_logLikelihood_cb_t),
       ('print_sample',CModel_print_sample_cb_t),
       ('copy',CModel_copy_cb_t),
       ('delete',CModel_del_cb_t),
       ('description',CModel_description_cb_t),
]

def _generate_callback(cb_t,func):
    return cb_t(func)

def CModel_delete(self_):
    del self_
        
#CSampler
lib.Sampler_new.restype = c_void_p
lib.Sampler_new.argtypes = [c_void_p,c_double,c_void_p]
lib.Sampler_delete.restype = None
lib.Sampler_delete.argtypes = [c_void_p]
lib.Sampler_loadLevels.restype = None
lib.Sampler_loadLevels.argtypes = [c_void_p,c_char_p]
lib.Sampler_run.restype = None
lib.Sampler_run.argtypes = [c_void_p]

class Sampler(object):

    def __init__(self,model,compression,options):
        
        self.sampler=lib.Sampler_new(model,compression,options.options)
        
    def __del__(self):
        lib.Sampler_delete(self.sampler)
        
    def loadLevels(self,filename):
        lib.Sampler_loadLevels(self.sampler,filename)

    def run(self):
        lib.Sampler_run(self.sampler)

            
#Utilities
lib.create_ostream_handle_for_file.restype=c_void_p
lib.create_ostream_handle_for_file.argtypes=[c_char_p]

def create_ostream_handle_for_file(filename):
    ostream_handle=lib.create_ostream_handle_for_file(filename)
    return ostream_handle

lib.print_string_to_stream.restype=None
lib.print_string_to_stream.argtypes=[c_void_p,c_char_p]

def print_string_to_stream(ostream_handle,s):
    lib.print_string_to_stream(ostream_handle,s)

# Python API
def initialise_rng():
    lib.RandomNumberGenerator_initialise_instance()

def generate_model(
    fromPrior,
    perturb,
    logLikelihood,
    print_sample,
    copy,
    description,
    state,
):

    MyModel_fromPrior_cb=_generate_callback(CModel_fromPrior_cb_t,fromPrior)
    MyModel_perturb_cb=_generate_callback(CModel_perturb_cb_t,perturb)
    MyModel_logLikelihood_cb=_generate_callback(CModel_logLikelihood_cb_t,logLikelihood)
    MyModel_print_sample_cb=_generate_callback(CModel_print_sample_cb_t,print_sample)
    MyModel_copy_cb=_generate_callback(CModel_copy_cb_t,copy)
    MyModel_description_cb=_generate_callback(CModel_description_cb_t,description)

    CModel_delete_cb=_generate_callback(CModel_del_cb_t,CModel_delete)

    model_methods=CModelMethods(
        MyModel_fromPrior_cb,
        MyModel_perturb_cb,
        MyModel_logLikelihood_cb,
        MyModel_print_sample_cb,
        MyModel_copy_cb,
        CModel_delete_cb,
        MyModel_description_cb,
    )

    weakrefs=[
        fromPrior,MyModel_fromPrior_cb,CModel_fromPrior_cb_t,
        perturb,MyModel_perturb_cb,CModel_perturb_cb_t,
        logLikelihood,MyModel_logLikelihood_cb,CModel_logLikelihood_cb_t,
        print_sample,MyModel_print_sample_cb,CModel_print_sample_cb_t,
        copy,MyModel_copy_cb,CModel_copy_cb_t,
        description,MyModel_description_cb,CModel_description_cb_t,
    ]

    def cb_mem_error(_):
        raise RuntimeError("CRITICAL ERROR: Lost ref to callback functions!")
    
    return CModel(pointer(model_methods),pointer(state),(weakrefs,[weakref.ref(i) for i in weakrefs]))
