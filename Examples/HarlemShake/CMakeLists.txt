set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/Examples/HarlemShake")
add_executable (HarlemShake HarlemShake.cpp main.cpp)
set_target_properties (HarlemShake PROPERTIES OUTPUT_NAME main)
target_link_libraries (HarlemShake dnest3 ${DNEST_DEPS})
