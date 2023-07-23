cmake_minimum_required(VERSION 3.5)
include(FindPackageHandleStandardArgs)

# Set project name and version
project(LAB04 VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use vcpkg toolchain file
set(VCPKG_TARGET_TRIPLET "x64-windows")
get_filename_component(PARENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
get_filename_component(PARENT_DIR "${PARENT_DIR}" DIRECTORY)
get_filename_component(PARENT_DIR "${PARENT_DIR}" DIRECTORY)
set(VCPKG_ROOT "${PARENT_DIR}")
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/vcpkg/scripts/buildsystems/vcpkg.cmake")

link_directories("${VCPKG_ROOT}/vcpkg/packages")
# Find required libraries
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_path(
        GLM_INCLUDE_DIR
        NAMES glm/glm.hpp
        PATHS
        ${VCPKG_ROOT}/vcpkg/installed/x64-windows/include
        DOC "The directory where glm/glm.hpp resides")
find_package_handle_standard_args(GLM DEFAULT_MSG GLM_INCLUDE_DIR)
set(GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIR})


# Set include directories
include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
# Add executable
add_executable(LAB04
        LAB04/argparser.h
        LAB04/bag.h
        LAB04/boundingbox.cpp
        LAB04/boundingbox.h
        LAB04/camera.cpp
        LAB04/camera.h
        LAB04/edge.cpp
        LAB04/edge.h
        LAB04/face.cpp
        LAB04/face.h
        LAB04/glCanvas.cpp
        LAB04/glCanvas.h
        LAB04/hit.h
        LAB04/LAB04.cpp
        LAB04/LAB04.h
        LAB04/material.cpp
        LAB04/material.h
        LAB04/matrix.cpp
        LAB04/matrix.h
        LAB04/mesh.cpp
        LAB04/mesh.h
        LAB04/radiosity.cpp
        LAB04/radiosity.h
        LAB04/ray.h
        LAB04/raytracer.h
        LAB04/raytracer_students.cpp
        LAB04/raytree.cpp
        LAB04/raytree.h
        LAB04/sphere.cpp
        LAB04/sphere.h
        LAB04/utils.h
        LAB04/vectors.h
        LAB04/vertex.h
        LAB04/vertex_parent.h)


# Link required libraries
target_link_libraries(LAB04 ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLUT_LIBRARIES})
