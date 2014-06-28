set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Examples/SpikeSlab")
add_executable (SpikeSlab SpikeSlab.cpp main.cpp)
set_target_properties (SpikeSlab PROPERTIES OUTPUT_NAME main)
target_link_libraries (SpikeSlab dnest3 ${DNEST_DEPS})

