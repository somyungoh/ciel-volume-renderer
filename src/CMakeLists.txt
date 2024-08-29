cmake_minimum_required(VERSION 3.12)

add_subdirectory(math)
add_subdirectory(volume)

# Search Paths
include_directories(${GLFW_INCLUDE_DIRS})

add_executable(CielApp
    cielApp.cpp
    renderer.cpp
    main.cpp
    scene.cpp
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

# ImGui
if(CIEL_BUILD_WITH_IMGUI)
    target_include_directories(CielApp PRIVATE ${CMAKE_HOME_DIRECTORY}/external/imgui)
    target_link_libraries(CielApp PRIVATE CielImGui)
endif()

# other properties
set_property(TARGET CielApp PROPERTY CXX_STANDARD 23)
