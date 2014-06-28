set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Examples/FitSine")
add_executable (FitSine FitSine.cpp Data.cpp main.cpp)
target_link_libraries (FitSine dnest3 ${DNEST_DEPS})

