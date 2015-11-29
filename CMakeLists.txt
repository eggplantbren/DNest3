project (DNest3 C CXX)
cmake_minimum_required (VERSION 2.8.0)

# Setting this explicitly avoids warnings from versions of CMake >= 3
# regarding policy CMP0042; it should be ignored versions of CMake which don't
# support it. (Setting the policy explicitly will break with old CMakes.)
set (CMAKE_MACOSX_RPATH TRUE)

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set (CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -ansi -Wno-unused-parameter")
set (CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set (CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "release")
endif (NOT CMAKE_BUILD_TYPE)

# Initialize the dependencies to an empty list.
set (DNEST_DEPS)

# ==========================================================================
#                                                                        GSL
# ==========================================================================

find_package (GSL REQUIRED)
list (APPEND DNEST_DEPS ${GSL_LIBRARIES})
include_directories (SYSTEM ${GSL_INCLUDES})

# ==========================================================================
#                                                                      BOOST
# ==========================================================================

find_package (Boost COMPONENTS thread system iostreams)
if (Boost_FOUND)
    list (APPEND DNEST_DEPS ${Boost_LIBRARIES})
    include_directories (SYSTEM ${Boost_INCLUDES})

    # try and find zlib if available
    find_package (ZLIB)
    if (ZLIB_FOUND)
        add_definitions (-DDNest3_zlib)
    endif (ZLIB_FOUND)
else (Boost_FOUND)
    add_definitions (-DDNest3_No_Boost)
endif (Boost_FOUND)

# ==========================================================================
#                                                                THE LIBRARY
# ==========================================================================

include_directories (include)

set (DNEST_SRC
     src/CommandLineOptions.cpp
     src/Level.cpp
     src/LikelihoodType.cpp
     src/Model.cpp
     src/Options.cpp
     src/RandomNumberGenerator.cpp
     src/Utils.cpp)

set (DNEST_INCLUDES
     include/CommandLineOptions.h
     include/Level.h
     include/LikelihoodType.h
     include/MTSampler.h
     include/MTSamplerImpl.h
     include/Model.h
     include/Options.h
     include/RandomNumberGenerator.h
     include/Sampler.h
     include/SamplerImpl.h
     include/Start.h
     include/StartImpl.h
     include/Utils.h
)

add_library (dnest3 ${DNEST_SRC})
target_link_libraries (dnest3 ${DNEST_DEPS})

install (TARGETS dnest3 DESTINATION lib)
install (FILES ${DNEST_INCLUDES} DESTINATION include/dnest3)

# ==========================================================================
#                                                               THE EXAMPLES
# ==========================================================================

add_subdirectory (Examples)

# ==========================================================================
#                                                          THE DOCUMENTATION
# ==========================================================================

if (BUILD_MANUAL)
    add_subdirectory (doc)
endif (BUILD_MANUAL)
