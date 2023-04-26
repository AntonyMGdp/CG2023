cmake_minimum_required(VERSION 3.5)
include(FindPackageHandleStandardArgs)

# Set project name and version
project(lab03 VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use vcpkg toolchain file
set(VCPKG_TARGET_TRIPLET "x64-windows")
set(CMAKE_TOOLCHAIN_FILE "C:/Users/Tello/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Find required libraries
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_path(
        GLM_INCLUDE_DIR
        NAMES glm/glm.hpp
        PATHS
        C:/Users/Tello/vcpkg/installed/x64-windows/include
        DOC "The directory where glm/glm.hpp resides")
find_package_handle_standard_args(GLM DEFAULT_MSG GLM_INCLUDE_DIR)
set(GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIR})


# Add executable
add_executable(lab03
        lab03/LAB03.cpp
        lab03/ShaderMaker.cpp
        lab03/HUD_Logger.cpp
        lab03/HUD_Logger.h)


# Set include directories
include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

# Link required libraries
target_link_libraries(lab03 ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLUT_LIBRARIES})