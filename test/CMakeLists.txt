find_package(Catch2 REQUIRED)
find_package(GooseFEM REQUIRED)

set(test_name "main")

add_executable(${test_name} main.cpp)

target_link_libraries(main PRIVATE
    Catch2::Catch2
    GooseFEM
    XDMFWrite_HighFive
    XDMFWrite_HighFive::compiler_warnings
    XDMFWrite_HighFive::assert)

add_test(NAME ${test_name} COMMAND ${test_name})
