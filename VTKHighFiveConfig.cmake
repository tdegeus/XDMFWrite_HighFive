# VTKHighFive cmake module
#
# This module sets the target:
#
#   VTKHighFive
#
# In addition, it sets the following variables:
#
#   VTKHighFive_FOUND - true if VTKHighFive found
#   VTKHighFive_VERSION - VTKHighFive's version
#   VTKHighFive_INCLUDE_DIRS - the directory containing VTKHighFive headers
#
# The following support targets are defined to simplify things:
#
#   VTKHighFive::compiler_warnings - enable compiler warnings
#   VTKHighFive::assert - enable VTKHighFive assertions

include(CMakeFindDependencyMacro)

# Define target "VTKHighFive"

if(NOT TARGET VTKHighFive)
    include("${CMAKE_CURRENT_LIST_DIR}/VTKHighFiveTargets.cmake")
    get_target_property(VTKHighFive_INCLUDE_DIRS VTKHighFive INTERFACE_INCLUDE_DIRECTORIES)
endif()

# Find dependencies

set(HIGHFIVE_USE_BOOST 0)
find_package(HighFive REQUIRED)

# Define support target "VTKHighFive::compiler_warnings"

if(NOT TARGET VTKHighFive::compiler_warnings)
    add_library(VTKHighFive::compiler_warnings INTERFACE IMPORTED)
    if(MSVC)
        set_property(
            TARGET VTKHighFive::compiler_warnings
            PROPERTY INTERFACE_COMPILE_OPTIONS
            /W4)
    else()
        set_property(
            TARGET VTKHighFive::compiler_warnings
            PROPERTY INTERFACE_COMPILE_OPTIONS
            -Wall -Wextra -pedantic -Wno-unknown-pragmas)
    endif()
endif()

# Define support target "VTKHighFive::assert"

if(NOT TARGET VTKHighFive::assert)
    add_library(VTKHighFive::assert INTERFACE IMPORTED)
    set_property(
        TARGET VTKHighFive::assert
        PROPERTY INTERFACE_COMPILE_DEFINITIONS
        VTKHIGHFIVE_ENABLE_ASSERT)
endif()
