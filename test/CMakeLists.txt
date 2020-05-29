
cmake_minimum_required(VERSION 3.0)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project(VTKHighFive-test)
    find_package(VTKHighFive REQUIRED CONFIG)
endif()

option(WARNINGS "Show build warnings" ON)
option(ASSERT "Enable assertions" ON)

find_package(Catch2 REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE Catch2::Catch2 VTKHighFive)

if (WARNINGS)
    target_link_libraries(main PRIVATE VTKHighFive::compiler_warnings)
endif()

if (ASSERT)
    target_link_libraries(main PRIVATE VTKHighFive::assert)
endif()
