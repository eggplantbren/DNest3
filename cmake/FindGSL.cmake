find_path (GSL_INCLUDES
    NAMES "gsl/gsl_randist.h" "gsl/gsl_rng.h"
    PATH_SUFFIXES "/include"
)

find_library (GSL_LIB
    NAMES gsl
    PATH_SUFFIXES "/lib"
)
list (APPEND GSL_LIBRARIES ${GSL_LIB})

find_library (GSLCBLAS_LIB
    NAMES gslcblas
    PATH_SUFFIXES "/lib"
)
list (APPEND GSL_LIBRARIES ${GSLCBLAS_LIB})

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (GSL
    DEFAULT_MESG
    GSL_INCLUDES
    GSL_LIB
    GSLCBLAS_LIB
)
