function(compile_example exemple_source)

    get_filename_component(example_filename ${exemple_source} NAME)

    string(REPLACE ".cpp" "_bin" example_name ${example_filename})

    add_executable(${example_name} ${exemple_source})


    target_link_libraries(${example_name} PRIVATE
        XDMFWrite_HighFive
        XDMFWrite_HighFive::compiler_warnings
        XDMFWrite_HighFive::assert)

    add_test(NAME ${example_name} COMMAND ${example_name})

endfunction()

file(GLOB list_example "*.cpp")

foreach(example_src ${list_example})
    compile_example(${example_src})
endforeach()
