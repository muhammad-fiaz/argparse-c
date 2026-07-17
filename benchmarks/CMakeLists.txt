add_executable(bench_argparse benchmark.c)
target_link_libraries(bench_argparse PRIVATE argparse-c-static)
target_include_directories(bench_argparse PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(UNIX AND NOT APPLE)
  target_link_libraries(bench_argparse PRIVATE rt)
endif()
