cmake_minimum_required(VERSION 3.12)
project(Ciel VERSION 0.1.0 LANGUAGES CXX)

# Ciel CMake config
include(${CMAKE_HOME_DIRECTORY}/cmake/CielCompileOpts.cmake)
include(${CMAKE_HOME_DIRECTORY}/cmake/CielFindDeps.cmake)

# output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# build tests
include(CTest)
enable_testing()

# source
add_subdirectory(src)

# project info
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# export build/complie_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
