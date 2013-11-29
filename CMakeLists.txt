# Metadata for the build
CMAKE_MINIMUM_REQUIRED (VERSION 2.8.0)
PROJECT (DNest3 C CXX)
SET (THRESHER_VERSION 0.0.1)

# Initialize the dependencies to an empty list.
SET (DNEST_DEPS)

# Default search directories.
LIST (APPEND SEARCH_LIBS /usr/lib)
LIST (APPEND SEARCH_LIBS /usr/local/lib)
LIST (APPEND SEARCH_LIBS /usr/local/homebrew/lib)
LIST (APPEND SEARCH_LIBS /opt/local/lib)

LIST (APPEND SEARCH_HEADERS /usr/include)
LIST (APPEND SEARCH_HEADERS /usr/local/include)
LIST (APPEND SEARCH_HEADERS /usr/local/homebrew/include)
LIST (APPEND SEARCH_HEADERS /opt/local/include)


# ==========================================================================
#                                                                        GSL
# ==========================================================================

MESSAGE ("-- Check for GSL")
SET (GSL_FOUND TRUE)

SET (GSL_SEARCH_LIBS SEARCH_LIBS)
LIST (APPEND GSL_SEARCH_LIBS /usr/lib/gsl)
LIST (APPEND GSL_SEARCH_LIBS /usr/local/lib/gsl)
LIST (APPEND GSL_SEARCH_LIBS /usr/local/homebrew/lib/gsl)
LIST (APPEND GSL_SEARCH_LIBS /opt/local/lib)
FIND_LIBRARY (GSL_LIB NAMES gsl PATHS ${GSL_SEARCH_LIBS})
IF (NOT EXISTS ${GSL_LIB})
    MESSAGE(FATAL_ERROR "Can't find GSL. Try passing -DGSL_LIB=...")
ENDIF (NOT EXISTS ${GSL_LIB})
MESSAGE("-- Found GSL library: ${GSL_LIB}")
LIST (APPEND DNEST_DEPS ${GSL_LIB})

SET (GSL_SEARCH_HEADERS SEARCH_HEADERS)
LIST (APPEND GSL_SEARCH_HEADERS /usr/include/gsl)
LIST (APPEND GSL_SEARCH_HEADERS /usr/local/include/gsl)
LIST (APPEND GSL_SEARCH_HEADERS /usr/local/homebrew/include/gsl)
LIST (APPEND GSL_SEARCH_HEADERS /opt/local/include)
FIND_PATH (GSL_INCLUDE NAMES gsl/gsl_randist.h gsl/gsl_rng.h PATHS ${GSL_SEARCH_HEADERS})
IF (NOT EXISTS ${GSL_INCLUDE})
    MESSAGE(FATAL_ERROR "Can't find GSL headers. Try passing -DGSL_INCLUDE=...")
ENDIF (NOT EXISTS ${GSL_INCLUDE})
MESSAGE("-- Found GSL headers: ${GSL_INCLUDE}")


# ==========================================================================
#                                                                      BOOST
# ==========================================================================

MESSAGE ("-- Check for Boost (optional)")
SET (BOOST_FOUND TRUE)

SET (BOOST_SEARCH_HEADERS SEARCH_HEADERS)
LIST (APPEND BOOST_SEARCH_HEADERS /usr/include/boost)
LIST (APPEND BOOST_SEARCH_HEADERS /usr/local/include/boost)
LIST (APPEND BOOST_SEARCH_HEADERS /usr/local/homebrew/include/boost)
LIST (APPEND BOOST_SEARCH_HEADERS /opt/local/include)
FIND_PATH (BOOST_INCLUDE NAMES boost/thread.hpp boost/thread/tss.hpp boost/bind.hpp PATHS ${BOOST_SEARCH_HEADERS})
IF (EXISTS ${BOOST_INCLUDE})
    MESSAGE("-- Found Boost headers: ${BOOST_INCLUDE}")
ELSE (EXISTS ${BOOST_INCLUDE})
    MESSAGE("-- Didn't find Boost headers")
    SET (BOOST_FOUND FALSE)
    ADD_DEFINITIONS(-DDNest3_No_Boost)
ENDIF (EXISTS ${BOOST_INCLUDE})


# ==========================================================================
#                                                             THE EXECUTABLE
# ==========================================================================

INCLUDE_DIRECTORIES (include
                     src
                     ${GSL_INCLUDE}
                     ${BOOST_INCLUDE})

SET (DNEST_SRC
     src/CommandLineOptions.cpp
     src/Level.cpp
     src/LikelihoodType.cpp
     src/Model.cpp
     src/Options.cpp
     src/RandomNumberGenerator.cpp
     src/Utils.cpp)

ADD_LIBRARY (dnest3 STATIC ${DNEST_SRC})
TARGET_LINK_LIBRARIES (dnest3 ${DNEST_DEPS})

INSTALL (TARGETS dnest3 DESTINATION lib)
INSTALL (FILES include/CommandLineOptions.hpp
               include/Level.hpp
               include/LikelihoodType.hpp
               include/MTSampler.hpp
               include/MTSamplerImpl.hpp
               include/Model.hpp
               include/Options.hpp
               include/RandomNumberGenerator.hpp
               include/Sampler.hpp
               include/SamplerImpl.hpp
               include/Start.hpp
               include/StartImpl.hpp
               include/Utils.hpp
         DESTINATION include/dnest3)
