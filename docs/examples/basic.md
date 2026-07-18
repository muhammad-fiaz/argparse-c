---
title: "Basic Usage"
description: "Simple option and positional argument parsing. int main(int argc, const char **argv) { Hello, Alice! Hi, Bob! Hi, Bob! Hi, Bob!"
---

# Basic Usage

Simple option and positional argument parsing.

## Complete Example

```c
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("greet", "A greeting program");

    argparse_add_option(parser, 'n', "name", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Your name", "name");
    argparse_add_option(parser, 'c', "count", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Number of greetings", "count");
    argparse_add_option(parser, 'u', "uppercase", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Uppercase output", "uppercase");

    argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                            "Greeting message", "message");

    struct argparse_result *result = argparse_parse(parser, argc, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    const char *name = argparse_get_string(result, "name");
    int count = argparse_get_int(result, "count");
    bool uppercase = argparse_get_bool(result, "uppercase");
    const char *message = argparse_get_string(result, "message");

    if (count <= 0) count = 1;

    for (int i = 0; i < count; i++) {
        if (uppercase) {
            printf("%s, %s!\n", message, name ? name : "World");
        } else {
            printf("%s, %s!\n", message, name ? name : "World");
        }
    }

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Usage

```bash
$ ./greet --name Alice "Hello"
Hello, Alice!

$ ./greet -n Bob -c 3 "Hi"
Hi, Bob!
Hi, Bob!
Hi, Bob!
```

## Key Points

- `argparse_new()` creates the parser with program name and description
- `argparse_add_option()` adds optional arguments with short/long names
- `argparse_add_positional()` adds position-based arguments
- `argparse_parse()` returns a result with all parsed values
- Always free the result and parser when done
