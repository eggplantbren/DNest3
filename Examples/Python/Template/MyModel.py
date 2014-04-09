import ctypes
import DNest3

def MyModel_fromPrior(self_):
    return

def MyModel_perturb(self_):
    return 0.

def MyModel_logLikelihood(self_):
    return 1.

def MyModel_print_sample(self_,ostream_handle):
    return

def MyModel_copy(self_state,other_state):
    self_state=other_state

def MyModel_description(self_):
    return ""

class Data(ctypes.Structure):
    _fields_=[
        ('dummy',ctypes.c_void_p)
    ]
    
MyModel_state=Data()

model = DNest3.generate_model(
    MyModel_fromPrior,
    MyModel_perturb,
    MyModel_logLikelihood,
    MyModel_print_sample,
    MyModel_copy,
    MyModel_description,
    MyModel_state,
)
