cmake_minimum_required(VERSION 3.10)
project(OpenGL)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)

# I don't know why, but I had to include this
add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
) # Otherwise there is an undefined reference error in glew

# GLEW
include_directories(external/glew/include)
add_library(glew STATIC external/glew/src/glew.c)
target_link_libraries(glew ${OPENGL_LIBRARY})
# GLFW
include_directories(external/glfw/include/GLFW)
add_subdirectory(external/glfw)

# util
# vector
set(vector src/util/Vector.cpp src/util/Vector.h)

set(util
        ${vector})

add_executable(OpenGL src/main.cpp ${util})
target_link_libraries(OpenGL ${OPENGL_LIBRARY} glew glfw)

#copy shaders
file(COPY src/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})