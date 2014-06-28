add_executable (HarlemShake HarlemShake.cpp main.cpp)
target_link_libraries (HarlemShake dnest3 ${DNEST_DEPS})
configure_file (showresults.py showresults.py @COPYONLY)
configure_file (OPTIONS OPTIONS @COPYONLY)
