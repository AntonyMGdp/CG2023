cmake_minimum_required(VERSION 3.5)
include(FindPackageHandleStandardArgs)

# Set project name and version
project(DEMO2D VERSION 1.0)

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


# Add executable
add_executable(DEMO2D
        DEMO2D/DEMO2D.cpp
        DEMO2D/ShaderMaker.cpp
        DEMO2D/ShaderMaker.h)


# Set include directories
include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

# Link required libraries
target_link_libraries(DEMO2D ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLUT_LIBRARIES})