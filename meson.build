project(
  'argparse-c',
  'c',
  'cpp',
  version: '1.0.0',
  license: 'MIT',
  default_options: [
    'c_std=c99',
    'cpp_std=c++20',
    'warning_level=3',
    'werror=true',
  ],
  meson_version: '>=0.60.0',
)

cc = meson.get_compiler('cc')
cxx = meson.get_compiler('cpp')

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
build_shared = get_option('default_library') != 'static'
build_static = get_option('default_library') != 'shared'

# ---------------------------------------------------------------------------
# Sources
# ---------------------------------------------------------------------------
argparse_sources = files(
  'src/argparse_memory.c',
  'src/argparse_error.c',
  'src/argparse_version.c',
  'src/argparse_unicode.c',
  'src/argparse_argument.c',
  'src/argparse_group.c',
  'src/argparse_mutex.c',
  'src/argparse_command.c',
  'src/argparse_action.c',
  'src/argparse_validator.c',
  'src/argparse_namespace.c',
  'src/argparse_format.c',
  'src/argparse_config.c',
  'src/argparse_complete.c',
  'src/argparse_parser.c',
  'src/argparse_internal.c',
  'src/argparse_cpp.cpp',
)

argparse_inc = include_directories('include')

# ---------------------------------------------------------------------------
# Library
# ---------------------------------------------------------------------------
argparse_lib = library(
  'argparse-c',
  argparse_sources,
  include_directories: argparse_inc,
  install: true,
  gnu_symbol_visibility: 'default',
)

argparse_dep = declare_dependency(
  link_with: argparse_lib,
  include_directories: argparse_inc,
)

# ---------------------------------------------------------------------------
# Subdirectories
# ---------------------------------------------------------------------------
if get_option('tests')
  subdir('tests')
endif

if get_option('examples')
  subdir('examples')
endif

if get_option('benchmarks')
  subdir('benchmarks')
endif

# ---------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------
install_subdirectory(
  'include/argparse-c',
  install_dir: get_option('includedir'),
)

# pkg-config
pkg = import('pkgconfig')
pkg.generate(
  argparse_lib,
  description: 'A full-featured argument parsing library for C',
  url: 'https://github.com/muhammad-fiaz/argparse-c',
)
