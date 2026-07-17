<div align="center">

# argparse-c

<a href="https://muhammad-fiaz.github.io/argparse-c/"><img src="https://img.shields.io/badge/docs-muhammad--fiaz.github.io-blue" alt="Documentation"></a>
<a href="https://github.com/muhammad-fiaz/argparse-c"><img src="https://img.shields.io/badge/C-C99%20%7C%20C11%20%7C%20C17%20%7C%20C23-blue.svg?logo=c" alt="C Standard"></a>
<a href="https://github.com/muhammad-fiaz/argparse-c"><img src="https://img.shields.io/github/stars/muhammad-fiaz/argparse-c" alt="GitHub stars"></a>
<a href="https://github.com/muhammad-fiaz/argparse-c/issues"><img src="https://img.shields.io/github/issues/muhammad-fiaz/argparse-c" alt="GitHub issues"></a>
<a href="https://github.com/muhammad-fiaz/argparse-c/pulls"><img src="https://img.shields.io/github/issues-pr/muhammad-fiaz/argparse-c" alt="GitHub pull requests"></a>
<a href="https://github.com/muhammad-fiaz/argparse-c"><img src="https://img.shields.io/github/last-commit/muhammad-fiaz/argparse-c" alt="GitHub last commit"></a>
<a href="https://github.com/muhammad-fiaz/argparse-c"><img src="https://img.shields.io/github/license/muhammad-fiaz/argparse-c" alt="License"></a>
<a href="https://github.com/muhammad-fiaz/argparse-c/actions/workflows/ci.yml"><img src="https://github.com/muhammad-fiaz/argparse-c/actions/workflows/ci.yml/badge.svg" alt="CI"></a>
<img src="https://img.shields.io/badge/platforms-linux%20%7C%20windows%20%7C%20macos-blue" alt="Supported Platforms">
<a href="https://github.com/muhammad-fiaz/argparse-c/releases/latest"><img src="https://img.shields.io/github/v/release/muhammad-fiaz/argparse-c?label=Latest%20Release&style=flat-square" alt="Latest Release"></a>
<a href="https://pay.muhammadfiaz.com"><img src="https://img.shields.io/badge/Sponsor-pay.muhammadfiaz.com-ff69b4?style=flat&logo=heart" alt="Sponsor"></a>
<a href="https://github.com/sponsors/muhammad-fiaz"><img src="https://img.shields.io/badge/Sponsor-GitHub-pink?style=social&logo=github" alt="GitHub Sponsors"></a>
<a href="https://hits.sh/github.com/muhammad-fiaz/argparse-c/"><img src="https://hits.sh/github.com/muhammad-fiaz/argparse-c.svg?label=Visitors&extraCount=0&color=green" alt="Repo Visitors"></a>

<p><em>A modern, production-quality C/C++ argument parsing library inspired by Python's argparse (C99 and later).</em></p>

<b><a href="https://muhammad-fiaz.github.io/argparse-c/">Documentation</a> |
<a href="https://muhammad-fiaz.github.io/argparse-c/api/overview">API Reference</a> |
<a href="https://muhammad-fiaz.github.io/argparse-c/guide/getting-started">Quick Start</a> |
<a href="CONTRIBUTING.md">Contributing</a></b>

</div>

`argparse-c` is a modern, production-quality C/C++ argument parsing library inspired by Python's argparse. Compatible with C99, C11, C17, C23, and C++20.

> [!TIP]
> If you build with argparse-c, make sure to give it a star.

> [!NOTE]
> **Cross-platform:** argparse-c supports Linux (GCC, Clang), Windows (MSVC 2015+), and macOS (Clang) with zero external dependencies. C99 or later required, C++ fully supported.

---

<details>
<summary><strong>Features</strong> (click to expand)</summary>

| Feature | Description | Documentation |
|---------|-------------|---------------|
| **Positional Arguments** | Required and optional positional arguments with type validation. | https://muhammad-fiaz.github.io/argparse-c/guide/positional |
| **Optional Arguments** | Short (-v) and long (--verbose) options with aliases. | https://muhammad-fiaz.github.io/argparse-c/guide/options |
| **Type System** | STRING, INT, UINT, FLOAT, DOUBLE, BOOL, ENUM types with parsing. | https://muhammad-fiaz.github.io/argparse-c/guide/types |
| **Actions** | STORE, STORE_TRUE, STORE_FALSE, APPEND, COUNT, HELP, VERSION. | https://muhammad-fiaz.github.io/argparse-c/guide/actions |
| **Argument Groups** | Organize options into named groups for help output. | https://muhammad-fiaz.github.io/argparse-c/guide/groups |
| **Mutually Exclusive** | Enforce mutual exclusion between options. | https://muhammad-fiaz.github.io/argparse-c/guide/groups |
| **Subcommands** | Git-style nested command hierarchy with sub-parsers. | https://muhammad-fiaz.github.io/argparse-c/guide/subcommands |
| **Custom Validators** | Plug in custom validation functions for option values. | https://muhammad-fiaz.github.io/argparse-c/guide/validators |
| **Error Handling** | Detailed error messages with did-you-mean suggestions (Levenshtein). | https://muhammad-fiaz.github.io/argparse-c/guide/error-handling |
| **Custom Allocators** | Pluggable allocator interface for embedded systems. | https://muhammad-fiaz.github.io/argparse-c/guide/custom-allocators |
| **Environment Variables** | Auto-load option values from environment variables. | https://muhammad-fiaz.github.io/argparse-c/guide/env-vars |
| **Config Files** | Load options from KEY=VALUE config files. | https://muhammad-fiaz.github.io/argparse-c/guide/config-files |
| **Shell Completion** | Generate completions for Bash, Zsh, Fish, PowerShell. | https://muhammad-fiaz.github.io/argparse-c/guide/shell-completion |
| **C++ Wrapper** | Modern C++20 wrapper with RAII, std::optional, method chaining. | https://muhammad-fiaz.github.io/argparse-c/examples/cpp |
| **Unicode** | UTF-8 string support with proper character width detection. | https://muhammad-fiaz.github.io/argparse-c/guide/unicode |
| **Response Files** | Load arguments from @file expansion. | https://muhammad-fiaz.github.io/argparse-c/guide/config-files |
| **Cross-Platform** | Runs on Linux, Windows (MSVC), and macOS. | https://muhammad-fiaz.github.io/argparse-c/guide/installation |
| **Zero Dependencies** | Uses only the C standard library. | https://muhammad-fiaz.github.io/argparse-c/guide/installation |

</details>

----

<details>
<summary><strong>Prerequisites and Supported Platforms</strong> (click to expand)</summary>

<br>

## Prerequisites

| Requirement | Version | Notes |
|-------------|---------|-------|
| **C Compiler** | C99 or later (GCC 4.8+, Clang 3.0+, MSVC 2015+) | Supports C99, C11, C17, C23 |
| **CMake** | 3.14+ | Build system |
| **C++ Compiler** | C++20 (for C++ wrapper) | Optional |

---

## Supported Platforms

| Platform | x86_64 (64-bit) | aarch64 (ARM64) |
|----------|-----------------|-----------------|
| **Linux** | Yes | Yes |
| **Windows** | Yes | Yes |
| **macOS** | Yes | Yes (Apple Silicon) |

</details>

---

## Installation

### Method 1: CMake (FetchContent)

```cmake
include(FetchContent)
FetchContent_Declare(argparse-c
    GIT_REPOSITORY https://github.com/muhammad-fiaz/argparse-c.git
    GIT_TAG v0.1.0)
FetchContent_MakeAvailable(argparse-c)
target_link_libraries(your_target PRIVATE argparse-c::static)
```

### Method 2: CMake (Subdirectory)

```bash
git clone https://github.com/muhammad-fiaz/argparse-c.git
```

```cmake
add_subdirectory(argparse-c)
target_link_libraries(your_target PRIVATE argparse-c::static)
```

### Method 3: xmake

```lua
add_requires("argparse-c")
target("myapp")
    set_kind("binary")
    add_files("src/main.c")
    add_packages("argparse-c")
```

### Method 4: Conan

```ini
[requires]
argparse-c/0.1.0
```

### Method 5: vcpkg

```bash
vcpkg install argparse-c
```

### Method 6: Build from Source

```bash
git clone https://github.com/muhammad-fiaz/argparse-c.git
cd argparse-c
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
cmake --install build
```

---

## Quick Start

### C Usage

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("myapp", "A modern argument parser for C");

    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Enable verbose output", "verbose");
    argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Output file", "output");
    argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                            "Input file", "input");

    struct argparse_result *result = argparse_parse(parser, argc, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    const char *input = argparse_get_string(result, "input");
    const char *output = argparse_get_string(result, "output");
    int verbose = argparse_get_bool(result, "verbose");

    printf("Input: %s\n", input);
    if (output) printf("Output: %s\n", output);
    if (verbose) printf("Verbose mode enabled\n");

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

### C++ Usage

```cpp
#include <argparse-c/argparse.hpp>
#include <iostream>

int main(int argc, const char **argv) {
    argparse_c::Parser parser("myapp", "A modern argument parser for C++");

    parser.add_option('v', "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose output", "verbose");
    parser.add_option('o', "output", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output file", "output");
    parser.add_positional(ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Input file", "input");

    auto result = parser.parse(argc, argv);

    if (result.has_error()) {
        std::cerr << "Error: " << result.error() << std::endl;
        return 1;
    }

    auto input = result.get_string("input");
    auto output = result.get_string("output");
    auto verbose = result.get_bool("verbose");

    std::cout << "Input: " << input.value_or("") << std::endl;
    if (output) std::cout << "Output: " << *output << std::endl;
    if (verbose && *verbose) std::cout << "Verbose mode enabled" << std::endl;

    return 0;
}
```

---

## Examples

The `examples/` directory contains **14 comprehensive, runnable examples** demonstrating all features:

| Example | Description | Documentation |
|---------|-------------|---------------|
| [`basic`](examples/basic.c) | Simplest parser with one option + positional | https://muhammad-fiaz.github.io/argparse-c/examples/basic |
| [`flags`](examples/flags.c) | Store_true, store_false, count action | https://muhammad-fiaz.github.io/argparse-c/examples/flags |
| [`options`](examples/options.c) | All types (STRING, INT, UINT, FLOAT, DOUBLE, BOOL, ENUM) | https://muhammad-fiaz.github.io/argparse-c/guide/options |
| [`subcommands`](examples/subcommands.c) | Git-like CLI with commit/push/status | https://muhammad-fiaz.github.io/argparse-c/examples/subcommands |
| [`validators`](examples/validators.c) | Custom validators for port, email, positive int | https://muhammad-fiaz.github.io/argparse-c/examples/validators |
| [`groups`](examples/groups.c) | Display groups and mutex groups | https://muhammad-fiaz.github.io/argparse-c/examples/groups |
| [`enums`](examples/enums.c) | Enum choices with defaults | https://muhammad-fiaz.github.io/argparse-c/guide/types |
| [`arrays`](examples/arrays.c) | nargs=*, APPEND, nargs=? for multi-value | https://muhammad-fiaz.github.io/argparse-c/examples/arrays |
| [`env_config`](examples/env_config.c) | Environment variable prefix + per-option env | https://muhammad-fiaz.github.io/argparse-c/guide/env-vars |
| [`advanced`](examples/advanced.c) | Callbacks, store_const, required options | https://muhammad-fiaz.github.io/argparse-c/guide/advanced |
| [`error_handling`](examples/error_handling.c) | Error codes and detailed diagnostics | https://muhammad-fiaz.github.io/argparse-c/guide/error-handling |
| [`large_cli`](examples/large_cli.c) | Realistic build-tool CLI | https://muhammad-fiaz.github.io/argparse-c/examples/large-cli |
| [`cpp_basic`](examples/cpp_basic.cpp) | C++ wrapper usage | https://muhammad-fiaz.github.io/argparse-c/examples/cpp |

To run any example:
```bash
cmake -B build -DARGPARSE_BUILD_EXAMPLES=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/examples/Release/example_basic
```

---

## Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `ARGPARSE_BUILD_SHARED` | ON | Build shared library |
| `ARGPARSE_BUILD_STATIC` | ON | Build static library |
| `ARGPARSE_BUILD_TESTS` | OFF | Build unit tests |
| `ARGPARSE_BUILD_EXAMPLES` | OFF | Build examples |
| `ARGPARSE_BUILD_BENCHMARKS` | OFF | Build benchmarks |
| `ARGPARSE_WERROR` | OFF | Treat warnings as errors |

---

## Testing

Run the full test suite:
```bash
cmake -B build -DARGPARSE_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

---

## Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Add tests for new functionality
4. Ensure all tests pass
5. Submit a pull request

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

---

## License

MIT License - Copyright (c) 2026 Muhammad Fiaz - see [LICENSE](LICENSE) for details.
