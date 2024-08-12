cmake_minimum_required(VERSION 3.12)

if(APPLE)
    add_compile_definitions(GL_SILENCE_DEPRECATION=1)
endif()

# maximum warning level
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wreturn-type -pedantic) # -Werror
endif()

# OpenMP
set (CIEL_BUILD_WITH_OMP 1)
