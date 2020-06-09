# XDMFWrite_HighFive cmake module
#
# This module sets the target:
#
#   XDMFWrite_HighFive
#
# In addition, it sets the following variables:
#
#   XDMFWrite_HighFive_FOUND - true if XDMFWrite_HighFive found
#   XDMFWrite_HighFive_VERSION - XDMFWrite_HighFive's version
#   XDMFWrite_HighFive_INCLUDE_DIRS - the directory containing XDMFWrite_HighFive headers
#
# The following support targets are defined to simplify things:
#
#   XDMFWrite_HighFive::compiler_warnings - enable compiler warnings
#   XDMFWrite_HighFive::assert - enable XDMFWrite_HighFive assertions

include(CMakeFindDependencyMacro)

# Define target "XDMFWrite_HighFive"

if(NOT TARGET XDMFWrite_HighFive)
    include("${CMAKE_CURRENT_LIST_DIR}/XDMFWrite_HighFiveTargets.cmake")
    get_target_property(XDMFWrite_HighFive_INCLUDE_DIRS XDMFWrite_HighFive INTERFACE_INCLUDE_DIRECTORIES)
endif()

# Find dependencies

set(HIGHFIVE_USE_BOOST 0)
find_package(HighFive REQUIRED)

# Define support target "XDMFWrite_HighFive::compiler_warnings"

if(NOT TARGET XDMFWrite_HighFive::compiler_warnings)
    add_library(XDMFWrite_HighFive::compiler_warnings INTERFACE IMPORTED)
    if(MSVC)
        set_property(
            TARGET XDMFWrite_HighFive::compiler_warnings
            PROPERTY INTERFACE_COMPILE_OPTIONS
            /W4)
    else()
        set_property(
            TARGET XDMFWrite_HighFive::compiler_warnings
            PROPERTY INTERFACE_COMPILE_OPTIONS
            -Wall -Wextra -pedantic -Wno-unknown-pragmas)
    endif()
endif()

# Define support target "XDMFWrite_HighFive::assert"

if(NOT TARGET XDMFWrite_HighFive::assert)
    add_library(XDMFWrite_HighFive::assert INTERFACE IMPORTED)
    set_property(
        TARGET XDMFWrite_HighFive::assert
        PROPERTY INTERFACE_COMPILE_DEFINITIONS
        XDMFWRITE_HIGHFIVE_ENABLE_ASSERT)
endif()
