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
# GLM
include_directories(external/glm)

function(add_source)
    foreach (arg IN LISTS ARGN)
        if (NOT ${arg} MATCHES ${ARGV0})
            get_filename_component(n ${arg} NAME)
            get_filename_component(p ${arg} PATH)
            set(f ${f}
                    src/${p}/${n}/${n}.cpp
                    src/${p}/${n}/${n}.h)
        endif ()
    endforeach ()

    set(${ARGV0} ${f} PARENT_SCOPE)
endfunction()

add_source(includes
#        GLContext
        GLWindow
#        GLContext/GLScene
        GLContext/GLInputHandler
        GLObject
        GLModel
        GLModel/Mesh
        GLModel/Material
        light
        Program
        UniBlock
        Camera
        Texture
        meshes/Grid
        meshes/Cube
        )

set(util
        src/util/util.h
        src/util/util.cpp
        src/util/loaders.cpp
        src/util/uniforms.h)

add_executable(OpenGL src/main.cpp
        "${includes}" ${util})

target_link_libraries(OpenGL
        ${OPENGL_LIBRARY}
        glew
        glfw)

#copy resources
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/resources)
file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

