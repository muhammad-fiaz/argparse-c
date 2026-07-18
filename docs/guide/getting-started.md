---
title: Getting Started
description: Get up and running with argparse-c in minutes
---

# Getting Started

Welcome to **argparse-c**, a production-quality C/C++ argument parsing library for modern applications.

## What is argparse-c?

argparse-c provides a clean, intuitive API for parsing command-line arguments in C and C++ programs. It handles:

- **Positional arguments** - Required and optional values
- **Options/Flags** - Named switches with short and long forms
- **Subcommands** - Nested command structures like `git commit`
- **Type validation** - Automatic type checking and conversion
- **Help generation** - Beautiful, customizable help text
- **Shell completion** - Bash, Zsh, and Fish support

## Quick Example

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("myapp", "A sample application");

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

    if (argparse_get_bool(result, "verbose")) {
        printf("Verbose mode enabled\n");
    }

    printf("Input: %s\n", argparse_get_string(result, "input"));

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Next Steps

- [Installation](./installation) - Add argparse-c to your project
- [Quick Start](./quick-start) - Build your first CLI in 5 minutes
- [Parser](./parser) - Deep dive into parser configuration
