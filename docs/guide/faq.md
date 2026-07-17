# FAQ

Frequently asked questions about argparse-c.

## General

### What is argparse-c?

argparse-c is a production-quality C/C++ argument parsing library. It provides a clean API for defining command-line options, positional arguments, subcommands, and more.

### What C standard does it require?

argparse-c requires C99 or later. The C++ wrapper requires C++20.

### Does it have any dependencies?

No. argparse-c is a zero-dependency library. It uses only the C standard library.

### Is it thread-safe?

Parser instances are **not** thread-safe. Each thread should create its own parser. Parse results (`argparse_result`) are safe to read concurrently once created.

## Usage

### How do I make an option required?

```c
struct argparse_option *opt = argparse_add_option(parser, 'o', "output",
    ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output file", "output");
argparse_option_set_required(opt, true);
```

### How do I add a boolean flag?

```c
/* --verbose sets verbose to true */
argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Verbose output", "verbose");
```

### How do I handle unknown options?

Check the error code after parsing:

```c
struct argparse_result *result = argparse_parse(parser, argc, argv);
if (argparse_result_error_code(result) == ARGPARSE_ERROR_AMBIGUOUS_OPTION) {
    fprintf(stderr, "Unknown option: %s\n", argparse_result_error(result));
}
```

### Can I parse from a string instead of argv?

argparse-c parses from `argc`/`argv` arrays. To parse from a string, split it into an array first.

### How do I get all values for a multi-value option?

```c
argparse_add_option(parser, 'f', "files", ARGPARSE_NARGS_STAR,
                    ARGPARSE_TYPE_STRING, "Input files", "files");

/* After parsing */
size_t count = argparse_get_count(result, "files");
for (size_t i = 0; i < count; i++) {
    const char *file = argparse_get_string_at(result, "files", i);
    printf("File %zu: %s\n", i, file);
}
```

### How do I add subcommands?

```c
struct argparse_command *cmd = argparse_add_command(parser, "init",
    "Initialize project", init_callback);
struct argparse *sub_parser = argparse_command_get_parser(cmd);
argparse_add_option(sub_parser, 'f', "force", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Force init", NULL);
```

## Errors

### What does `ARGPARSE_ERROR_OUT_OF_MEMORY` mean?

A memory allocation failed. This can happen if:
- You're using a custom allocator that returned NULL
- The system is out of memory
- The parser has too many options/arguments

### How do I get detailed error information?

```c
const char *error = argparse_result_error(result);
argparse_error_code code = argparse_result_error_code(result);
fprintf(stderr, "Error (%d): %s\n", code, error);
```

### Can I customize error messages?

Not directly. The library generates standardized error messages. You can check the error code and provide your own messages in your application.

## Build

### How do I use it with CMake?

```cmake
include(FetchContent)
FetchContent_Declare(
  argparse-c
  GIT_REPOSITORY https://github.com/muhammad-fiaz/argparse-c.git
  GIT_TAG v1.0.0
)
FetchContent_MakeAvailable(argparse-c)
target_link_libraries(your_target PRIVATE argparse-c::argparse-c)
```

### How do I build only the static library?

```bash
cmake -B build -DARGPARSE_BUILD_SHARED=OFF -DARGPARSE_BUILD_STATIC=ON
```

### How do I run tests with sanitizers?

```bash
cmake -B build \
  -DCMAKE_C_FLAGS="-fsanitize=address,undefined" \
  -DARGPARSE_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build
```

## Contributing

### Where do I report bugs?

Use the [bug report template](https://github.com/muhammad-fiaz/argparse-c/issues/new?template=bug_report.yml) on GitHub.

### How do I run the formatter?

```bash
./scripts/format.sh
```

### How do I run static analysis?

```bash
./scripts/lint.sh
```
