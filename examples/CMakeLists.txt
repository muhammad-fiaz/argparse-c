# examples/CMakeLists.txt

# C examples
set(C_EXAMPLES
    basic
    flags
    options
    subcommands
    validators
    groups
    enums
    arrays
    env_config
    advanced
    advanced_features
    error_handling
    large_cli
)

foreach(example ${C_EXAMPLES})
    add_executable(example_${example} ${example}.c)
    target_include_directories(example_${example} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )
    if(TARGET argparse-c-static)
        target_link_libraries(example_${example} PRIVATE argparse-c-static)
    else()
        target_link_libraries(example_${example} PRIVATE argparse-c)
    endif()
    set_target_properties(example_${example} PROPERTIES
        OUTPUT_NAME ${example}
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
    )
endforeach()

# C++ example
if(CMAKE_CXX_COMPILER)
    add_executable(example_cpp_basic cpp_basic.cpp)
    target_include_directories(example_cpp_basic PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )
    if(TARGET argparse-c-static)
        target_link_libraries(example_cpp_basic PRIVATE argparse-c-static)
    else()
        target_link_libraries(example_cpp_basic PRIVATE argparse-c)
    endif()
    set_target_properties(example_cpp_basic PROPERTIES
        OUTPUT_NAME cpp_basic
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )
endif()
