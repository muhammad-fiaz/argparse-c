cmake_minimum_required(VERSION 3.14)

project(
  argparse-c
  VERSION 0.1.0
  LANGUAGES C CXX
  DESCRIPTION "A full-featured argument parsing library for C"
  HOMEPAGE_URL "https://github.com/argparse-c/argparse-c"
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
option(ARGPARSE_BUILD_SHARED "Build shared library" ON)
option(ARGPARSE_BUILD_STATIC "Build static library" ON)
option(ARGPARSE_BUILD_TESTS "Build tests" OFF)
option(ARGPARSE_BUILD_EXAMPLES "Build examples" OFF)
option(ARGPARSE_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(ARGPARSE_BUILD_DOCS "Build documentation" OFF)
option(ARGPARSE_BUILD_FUZZ "Build fuzz targets" OFF)
option(ARGPARSE_INSTALL "Generate install target" ON)
option(ARGPARSE_WERROR "Treat warnings as errors" OFF)

# ---------------------------------------------------------------------------
# Source files
# ---------------------------------------------------------------------------
set(ARGPARSE_SOURCES
  src/argparse_memory.c
  src/argparse_error.c
  src/argparse_version.c
  src/argparse_unicode.c
  src/argparse_argument.c
  src/argparse_group.c
  src/argparse_mutex.c
  src/argparse_command.c
  src/argparse_action.c
  src/argparse_validator.c
  src/argparse_validator_extra.c
  src/argparse_color.c
  src/argparse_lazy.c
  src/argparse_namespace.c
  src/argparse_format.c
  src/argparse_config.c
  src/argparse_complete.c
  src/argparse_parser.c
  src/argparse_internal.c
  src/argparse_batch.c
  src/argparse_thread.c
  src/argparse_plugin.c
  src/argparse_i18n.c
  src/argparse_manpage.c
  src/argparse_json.c
  src/argparse_cpp.cpp
)

set(ARGPARSE_PUBLIC_HEADERS
  include/argparse-c/argparse.h
  include/argparse-c/argparse_version.h
  include/argparse-c/argparse_types.h
  include/argparse-c/argparse_memory.h
  include/argparse-c/argparse_error.h
)

# ---------------------------------------------------------------------------
# Compiler flags
# ---------------------------------------------------------------------------
set(ARGPARSE_C_FLAGS "")
set(ARGPARSE_CXX_FLAGS "")

if(MSVC)
  list(APPEND ARGPARSE_C_FLAGS /W4)
  list(APPEND ARGPARSE_CXX_FLAGS /W4)
  if(ARGPARSE_WERROR)
    list(APPEND ARGPARSE_C_FLAGS /WX)
    list(APPEND ARGPARSE_CXX_FLAGS /WX)
  endif()
else()
  list(APPEND ARGPARSE_C_FLAGS -Wall -Wextra -Wpedantic)
  list(APPEND ARGPARSE_CXX_FLAGS -Wall -Wextra -Wpedantic)
  if(ARGPARSE_WERROR)
    list(APPEND ARGPARSE_C_FLAGS -Werror)
    list(APPEND ARGPARSE_CXX_FLAGS -Werror)
  endif()
  if(NOT APPLE AND NOT CMAKE_CROSSCOMPILING)
    list(APPEND ARGPARSE_C_FLAGS -fstack-protector-strong)
    list(APPEND ARGPARSE_CXX_FLAGS -fstack-protector-strong)
  endif()
endif()

# ---------------------------------------------------------------------------
# Static library
# ---------------------------------------------------------------------------
if(ARGPARSE_BUILD_STATIC)
  add_library(argparse-c-static STATIC ${ARGPARSE_SOURCES})
  add_library(argparse-c::static ALIAS argparse-c-static)

  target_include_directories(
    argparse-c-static
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
  )

  target_compile_features(argparse-c-static PUBLIC c_std_99)
  target_compile_features(argparse-c-static PRIVATE cxx_std_20)

  target_compile_options(argparse-c-static PRIVATE ${ARGPARSE_C_FLAGS})
  target_compile_options(argparse-c-static PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${ARGPARSE_CXX_FLAGS}>)

  set_target_properties(
    argparse-c-static PROPERTIES
      OUTPUT_NAME argparse-c
      POSITION_INDEPENDENT_CODE ON
      C_STANDARD_REQUIRED ON
      C_EXTENSIONS OFF
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF
      DEFINE_SYMBOL ARGPARSE_STATIC_DEFINE
  )
endif()

# ---------------------------------------------------------------------------
# Shared library
# ---------------------------------------------------------------------------
if(ARGPARSE_BUILD_SHARED)
  add_library(argparse-c SHARED ${ARGPARSE_SOURCES})
  add_library(argparse-c::argparse-c ALIAS argparse-c)

  target_include_directories(
    argparse-c
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
  )

  target_compile_features(argparse-c PUBLIC c_std_99)
  target_compile_features(argparse-c PRIVATE cxx_std_20)

  target_compile_options(argparse-c PRIVATE ${ARGPARSE_C_FLAGS})
  target_compile_options(argparse-c PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${ARGPARSE_CXX_FLAGS}>)

  set_target_properties(
    argparse-c PROPERTIES
      VERSION ${PROJECT_VERSION}
      SOVERSION ${PROJECT_VERSION_MAJOR}
      POSITION_INDEPENDENT_CODE ON
      C_STANDARD_REQUIRED ON
      C_EXTENSIONS OFF
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF
      PUBLIC_HEADER "${ARGPARSE_PUBLIC_HEADERS}"
      WINDOWS_EXPORT_ALL_SYMBOLS ON
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  )

  target_compile_definitions(
    argparse-c
    PRIVATE ARGPARSE_BUILD_SHARED
    PUBLIC $<$<NOT:$<BOOL:${ARGPARSE_BUILD_STATIC}>>:ARGPARSE_STATIC_DEFINE>
  )
endif()

# ---------------------------------------------------------------------------
# Default target selection when both are built
# ---------------------------------------------------------------------------
if(ARGPARSE_BUILD_STATIC)
  add_library(argparse-c::argparse-c-alias ALIAS argparse-c-static)
elseif(ARGPARSE_BUILD_SHARED)
  add_library(argparse-c::argparse-c-alias ALIAS argparse-c)
endif()

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
if(ARGPARSE_BUILD_TESTS)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  enable_testing()
  add_subdirectory(tests)
endif()

# ---------------------------------------------------------------------------
# Examples
# ---------------------------------------------------------------------------
if(ARGPARSE_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# ---------------------------------------------------------------------------
# Benchmarks
# ---------------------------------------------------------------------------
if(ARGPARSE_BUILD_BENCHMARKS)
  add_subdirectory(benchmarks)
endif()

# ---------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------
if(ARGPARSE_INSTALL)
  set(ARGPARSE_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/argparse-c)

  if(ARGPARSE_BUILD_STATIC)
    install(
      TARGETS argparse-c-static
      EXPORT argparse-c-targets
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
  endif()

  if(ARGPARSE_BUILD_SHARED)
    install(
      TARGETS argparse-c
      EXPORT argparse-c-targets
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/argparse-c
    )
  endif()

  install(
    DIRECTORY include/argparse-c
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
  )

  # Generate package config files
  configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ArgparseConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/ArgparseConfig.cmake"
    INSTALL_DESTINATION ${ARGPARSE_INSTALL_CMAKEDIR}
  )

  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/ArgparseConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
  )

  install(
    FILES
      "${CMAKE_CURRENT_BINARY_DIR}/cmake/ArgparseConfig.cmake"
      "${CMAKE_CURRENT_BINARY_DIR}/cmake/ArgparseConfigVersion.cmake"
    DESTINATION ${ARGPARSE_INSTALL_CMAKEDIR}
  )

  install(
    EXPORT argparse-c-targets
    NAMESPACE argparse-c::
    DESTINATION ${ARGPARSE_INSTALL_CMAKEDIR}
  )

  # pkg-config
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/argparse-c.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/argparse-c.pc"
    @ONLY
  )

  install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/argparse-c.pc"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
  )
endif()
