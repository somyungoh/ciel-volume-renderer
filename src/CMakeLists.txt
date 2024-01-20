cmake_minimum_required(VERSION 3.12)

add_subdirectory(math)
add_subdirectory(volume)

# Search Paths
include_directories(${GLFW_INCLUDE_DIRS})

add_executable(CielApp
    cielApp.cpp
    Renderer.cpp
    main.cpp
)

# linking
target_link_libraries(CielApp PRIVATE glfw)
if(APPLE)
    target_link_libraries(CielApp PRIVATE ${COCOA_LIBRARY})
    target_link_libraries(CielApp PRIVATE ${IOKIT_LIBRARY})
    target_link_libraries(CielApp PRIVATE ${OPENGL_LIBRARY})
endif()
if(OpenMP_CXX_FOUND)
    target_link_libraries(CielApp PUBLIC OpenMP::OpenMP_CXX)
endif()

# other properties
set_property(TARGET CielApp PROPERTY CXX_STANDARD 23)
