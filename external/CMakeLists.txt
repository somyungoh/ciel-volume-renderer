cmake_minimum_required(VERSION 3.12)

# ImGui
if(CIEL_BUILD_WITH_IMGUI)
    add_subdirectory(imgui)
endif()