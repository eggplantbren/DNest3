set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Examples/FitSine")
add_executable (FitSine FitSine.cpp Data.cpp main.cpp)
set_target_properties (FitSine PROPERTIES OUTPUT_NAME main)
target_link_libraries (FitSine dnest3 ${DNEST_DEPS})

