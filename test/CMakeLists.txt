find_package(Catch2 REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE
    Catch2::Catch2
    XDMFWrite_HighFive
    XDMFWrite_HighFive::compiler_warnings
    XDMFWrite_HighFive::assert)
