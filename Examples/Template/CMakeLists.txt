set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Examples/Template")
add_executable (Template MyModel.cpp main.cpp)
target_link_libraries (Template dnest3 ${DNEST_DEPS})

