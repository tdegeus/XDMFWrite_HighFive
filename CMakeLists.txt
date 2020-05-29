#
# (c - GPLv3) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | github.com/tdegeus/VTKHighFive
#

cmake_minimum_required(VERSION 3.0)

# Basic settings
# ==============

project(VTKHighFive)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

# Version
# =======

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/include/VTKHighFive.h" VTKHighFive_version_defines
     REGEX "#define VTKHIGHFIVE_VERSION_(MAJOR|MINOR|PATCH)")

foreach(ver ${VTKHighFive_version_defines})
    if(ver MATCHES "#define VTKHIGHFIVE_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(VTKHIGHFIVE_VERSION_${CMAKE_MATCH_1}
            "${CMAKE_MATCH_2}"
            CACHE INTERNAL "")
    endif()
endforeach()

set(VTKHIGHFIVE_VERSION ${VTKHIGHFIVE_VERSION_MAJOR}.${VTKHIGHFIVE_VERSION_MINOR}.${VTKHIGHFIVE_VERSION_PATCH})

message(STATUS "Building VTKHighFive v${VTKHIGHFIVE_VERSION}")

# Set target
# ==========

set(HIGHFIVE_USE_BOOST 0)
find_package(HighFive REQUIRED)

add_library(VTKHighFive INTERFACE)

target_include_directories(VTKHighFive INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(VTKHighFive INTERFACE HighFive)
# target_compile_features(VTKHighFive INTERFACE cxx_std_14)

# Installation
# ============

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION include)

install(TARGETS VTKHighFive EXPORT VTKHighFive-targets)

install(
    EXPORT VTKHighFive-targets
    FILE VTKHighFiveTargets.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/VTKHighFive")

set(_VTKHIGHFIVE ${CMAKE_SIZEOF_VOID_P})
unset(CMAKE_SIZEOF_VOID_P)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/VTKHighFiveConfigVersion.cmake"
    VERSION ${VTKHIGHFIVE_VERSION}
    COMPATIBILITY AnyNewerVersion)

set(CMAKE_SIZEOF_VOID_P ${_VTKHIGHFIVE})

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/VTKHighFiveConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/VTKHighFiveConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/VTKHighFive")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/VTKHighFive.pc.in"
               "${CMAKE_CURRENT_BINARY_DIR}/VTKHighFive.pc" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/VTKHighFive.pc"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig/")

# Add builds
# ==========

include("VTKHighFiveConfig.cmake")

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

# if(BUILD_EXAMPLES)
#     add_subdirectory(docs/examples)
# endif()
