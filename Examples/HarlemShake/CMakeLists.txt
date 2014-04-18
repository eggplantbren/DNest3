add_executable (HarlemShake HarlemShake.cpp main.cpp)
target_link_libraries (HarlemShake dnest3 ${DNEST_DEPS})
configure_file (OPTIONS OPTIONS @COPYONLY)
