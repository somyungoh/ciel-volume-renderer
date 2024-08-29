cmake_minimum_required(VERSION 3.12)

add_library(CielImGui
    imgui.cpp
    imgui_draw.cpp
    imgui_demo.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.cpp
)

# target_include_directories(CielImGui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_INCLUDE_CURRENT_DIR ON)

target_link_libraries(CielImGui PRIVATE glfw)
