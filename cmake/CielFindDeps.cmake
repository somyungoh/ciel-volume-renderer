cmake_minimum_required(VERSION 3.12)

find_package(glfw3 3.3 REQUIRED FATAL_ERROR)    # Required for GUI
# find_package(OpenMP)                # Not required, but impactful

find_library(COCOA_LIBRARY Cocoa REQUIRED)
find_library(IOKIT_LIBRARY IOKit REQUIRED)
find_library(OPENGL_LIBRARY OpenGL REQUIRED)
