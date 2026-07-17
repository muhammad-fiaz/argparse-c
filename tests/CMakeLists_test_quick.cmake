enable_testing()
add_executable(test_quick test_quick.c)
target_link_libraries(test_quick PRIVATE argparse-c-static)
target_include_directories(test_quick PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src)
