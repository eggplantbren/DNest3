add_executable (FitSine Data.cpp FitSine.cpp main.cpp)
target_link_libraries (FitSine dnest3 ${DNEST_DEPS})
configure_file (fake_data.txt fake_data.txt @COPYONLY)
configure_file (OPTIONS OPTIONS @COPYONLY)
