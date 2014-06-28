set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Examples/Template")
add_executable (Template MyModel.cpp main.cpp)
set_target_properties (Template PROPERTIES OUTPUT_NAME main)
target_link_libraries (Template dnest3 ${DNEST_DEPS})

