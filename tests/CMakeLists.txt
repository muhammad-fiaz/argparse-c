# tests/CMakeLists.txt

# Each test file is compiled as a standalone executable with its own main()
set(TEST_SOURCES
    test_basic.c
    test_options.c
    test_positional.c
    test_types.c
    test_actions.c
    test_groups.c
    test_commands.c
    test_errors.c
    test_suggestions.c
    test_format.c
    test_memory.c
    test_color.c
    test_thread.c
    test_lazy.c
    test_json.c
    test_manpage.c
    test_i18n.c
    test_batch.c
    test_plugin.c
    test_validators_extra.c
    test_config.c
)

foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source} ${CMAKE_CURRENT_SOURCE_DIR}/test_framework.h)
    target_include_directories(${test_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    if(TARGET argparse-c-static)
        target_link_libraries(${test_name} PRIVATE argparse-c-static)
    else()
        target_link_libraries(${test_name} PRIVATE argparse-c)
    endif()
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

# test_main.c links against the library but has its own main
add_executable(test_main test_main.c test_framework.h)
target_include_directories(test_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
if(TARGET argparse-c-static)
    target_link_libraries(test_main PRIVATE argparse-c-static)
else()
    target_link_libraries(test_main PRIVATE argparse-c)
endif()
add_test(NAME test_main COMMAND test_main)

# Convenience target to run all tests
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS ${TEST_SOURCES} test_main
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
