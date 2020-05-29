#
# (c - GPLv3) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | github.com/tdegeus/XDMFWrite_HighFive
#

cmake_minimum_required(VERSION 3.16)

# Basic settings
# ==============

project(XDMFWrite_HighFive)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

# Version
# =======

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/include/XDMFWrite_HighFive.hpp" XDMFWrite_HighFive_version_defines
     REGEX "#define XDMFWRITE_HIGHFIVE_VERSION_(MAJOR|MINOR|PATCH)")

foreach(ver ${XDMFWrite_HighFive_version_defines})
    if(ver MATCHES "#define XDMFWRITE_HIGHFIVE_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(XDMFWRITE_HIGHFIVE_VERSION_${CMAKE_MATCH_1}
            "${CMAKE_MATCH_2}"
            CACHE INTERNAL "")
    endif()
endforeach()

set(XDMFWRITE_HIGHFIVE_VERSION ${XDMFWRITE_HIGHFIVE_VERSION_MAJOR}.${XDMFWRITE_HIGHFIVE_VERSION_MINOR}.${XDMFWRITE_HIGHFIVE_VERSION_PATCH})

message(STATUS "Building XDMFWrite_HighFive v${XDMFWRITE_HIGHFIVE_VERSION}")

# Set target
# ==========

set(HIGHFIVE_USE_BOOST 0)
find_package(HighFive REQUIRED)

add_library(XDMFWrite_HighFive INTERFACE)

target_include_directories(XDMFWrite_HighFive INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(XDMFWrite_HighFive INTERFACE HighFive)
target_compile_features(XDMFWrite_HighFive INTERFACE cxx_std_14)

# Installation
# ============

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION include)

install(TARGETS XDMFWrite_HighFive EXPORT XDMFWrite_HighFive-targets)

install(
    EXPORT XDMFWrite_HighFive-targets
    FILE XDMFWrite_HighFiveTargets.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/XDMFWrite_HighFive")

set(_XDMFWRITE_HIGHFIVE ${CMAKE_SIZEOF_VOID_P})
unset(CMAKE_SIZEOF_VOID_P)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/XDMFWrite_HighFiveConfigVersion.cmake"
    VERSION ${XDMFWRITE_HIGHFIVE_VERSION}
    COMPATIBILITY AnyNewerVersion)

set(CMAKE_SIZEOF_VOID_P ${_XDMFWRITE_HIGHFIVE})

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/XDMFWrite_HighFiveConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/XDMFWrite_HighFiveConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/XDMFWrite_HighFive")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/XDMFWrite_HighFive.pc.in"
               "${CMAKE_CURRENT_BINARY_DIR}/XDMFWrite_HighFive.pc" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/XDMFWrite_HighFive.pc"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig/")

# Add builds
# ==========

include("XDMFWrite_HighFiveConfig.cmake")

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(docs/examples)
endif()
