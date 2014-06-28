add_executable (SpikeSlab SpikeSlab.cpp main.cpp)
target_link_libraries (SpikeSlab dnest3 ${DNEST_DEPS})
configure_file (OPTIONS OPTIONS @COPYONLY)
