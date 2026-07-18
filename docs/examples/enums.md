---
title: "Enums"
description: "Demonstrates using `ARGPARSE_TYPE_ENUM` with `argparse_option_set_choices()` to restrict option values to a predefined set. int main(void) {"
---

# Enums

Demonstrates using `ARGPARSE_TYPE_ENUM` with `argparse_option_set_choices()` to restrict option values to a predefined set.

## Running

```bash
./enums --color red
./enums --color blue
./enums --color yellow   # Error: invalid choice
./enums                  # Uses default "auto"
```

## What It Demonstrates

- Enum choices with `argparse_option_set_choices()`
- Default values for enums
- Multiple enum options (color, log level)
- Error messages for invalid choices

## Full Code

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(void) {
    struct argparse *parser = argparse_new("enums", "Demonstrate enum options");

    struct argparse_option *color_opt = argparse_add_option(
        parser, 'c', "color", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Color", "color");
    argparse_option_set_default(color_opt, "auto");
    const char *colors[] = {"red", "green", "blue", "auto"};
    argparse_option_set_choices(color_opt, colors, 4);

    struct argparse_option *level_opt = argparse_add_option(
        parser, 'l', "level", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Log level", "level");
    argparse_option_set_default(level_opt, "info");
    const char *levels[] = {"debug", "info", "warn", "error"};
    argparse_option_set_choices(level_opt, levels, 4);

    const char *argv[] = {"enums", "--color", "red", "--level", "debug"};
    struct argparse_result *result = argparse_parse(parser, 5, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    printf("color: %s\n", argparse_get_string(result, "color"));
    printf("level: %s\n", argparse_get_string(result, "level"));

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```
