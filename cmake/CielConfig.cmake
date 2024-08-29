cmake_minimum_required(VERSION 3.24)

# output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# build tests
include(CTest)
enable_testing()

include_directories(
    ${CMAKE_HOME_DIRECTORY}/src
)
