---
title: Quick Start
description: Build your first CLI application in 5 minutes
---

# Quick Start

This guide walks you through building a complete CLI application with argparse-c in under 5 minutes.

## Step 1: Create the Project

```bash
mkdir mycli && cd mycli
mkdir -p include/argparse-c src
```

## Step 2: Add argparse-c

Copy `argparse.h` to `include/argparse-c/` and `argparse.c` to `src/`.

## Step 3: Write Your Application

Create `main.c`:

```c
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char **argv) {
    /* Create a parser */
    struct argparse *parser = argparse_new(
        "greeter",
        "A friendly greeting application"
    );

    /* Add options */
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Enable verbose output", NULL);
    argparse_add_option(parser, 'u', "uppercase", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Uppercase the greeting", NULL);
    argparse_add_option(parser, 'n', "count", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Number of greetings", "N");

    /* Add positional arguments */
    argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                            "Name of the person to greet", "name");
    argparse_add_positional(parser, ARGPARSE_NARGS_0, ARGPARSE_TYPE_STRING,
                            "Optional greeting word", "greeting");

    /* Parse arguments */
    struct argparse_result *result = argparse_parse(
        parser, argc, argv
    );

    /* Handle errors */
    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return EXIT_FAILURE;
    }

    /* Handle --help and --version automatically */
    if (result == NULL || argparse_result_error_code(result) != ARGPARSE_ERROR_UNKNOWN) {
        argparse_result_free(result);
        argparse_free(parser);
        return EXIT_SUCCESS;
    }

    /* Extract arguments */
    const char *name = argparse_get_string(result, "name");
    const char *greeting = argparse_get_string(result, "greeting");
    int count = argparse_get_int(result, "count");
    int verbose = argparse_get_bool(result, "verbose");
    int uppercase = argparse_get_bool(result, "uppercase");

    /* Default values */
    if (!greeting) greeting = "Hello";
    if (count <= 0) count = 1;

    /* Execute */
    if (verbose) {
        printf("Greeting '%s' %d time(s) to '%s'\n", greeting, count, name);
    }

    for (int i = 0; i < count; i++) {
        if (uppercase) {
            printf("%s, %s!\n", greeting, name);
        } else {
            printf("%s, %s!\n", greeting, name);
        }
    }

    /* Cleanup */
    argparse_result_free(result);
    argparse_free(parser);

    return EXIT_SUCCESS;
}
```

## Step 4: Compile and Run

```bash
gcc -o greeter main.c src/argparse.c -Iinclude
```

## Step 5: Test It

```bash
# Basic usage
./greeter World
# Output: Hello, World!

# With options
./greeter -v -u -n 3 Alice
# Output: Greeting 'Hello' 3 time(s) to 'Alice'
#         HELLO, ALICE!
#         HELLO, ALICE!
#         HELLO, ALICE!

# Custom greeting
./greeter Bob "Hey there"
# Output: Hey there, Bob!

# Help
./greeter --help
# Usage: greeter [OPTIONS] NAME [GREETING]
#
# A friendly greeting application
#
# Options:
#   -v, --verbose     Enable verbose output
#   -u, --uppercase   Uppercase the greeting
#   -n, --count N     Number of greetings
#   -h, --help        Show this help message
#   -V, --version     Show version
```

## What Just Happened?

1. **`argparse_new()`** - Created a parser with program name and description
2. **`argparse_add_option()`** - Added command-line flags
3. **`argparse_add_positional()`** - Added required positional arguments
4. **`argparse_parse()`** - Parsed the command-line arguments
5. **`argparse_get_*()`** - Extracted typed values from the result
6. **`argparse_result_free()`** - Cleaned up the result
7. **`argparse_free()`** - Cleaned up the parser

## Next Steps

- [Parser](./parser) - Deep dive into parser options
- [Arguments](./arguments) - Learn about argument types
- [Options](./options) - Explore all option features
