---
title: "Options & Types"
description: "Demonstrates all option types with custom defaults. int main(void) {"
---

# Options & Types

Demonstrates all option types with custom defaults.

## Running

```bash
./options --name Alice --port 8080 --rate 3.14 --color blue
./options                          # Uses defaults
./options --help                   # Show all options
```

## What It Demonstrates

- **STRING** option with default value
- **INT** option with default value
- **UINT** option for unsigned integers
- **FLOAT** and **DOUBLE** options
- **BOOL** option
- **ENUM** option with `argparse_option_set_choices()`
- Setting defaults with `argparse_option_set_default()`

## Full Code

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(void) {
    struct argparse *parser = argparse_new("options", "Demonstrate all option types");

    struct argparse_option *name_opt = argparse_add_option(
        parser, 'n', "name", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "User name", "name");
    argparse_option_set_default(name_opt, "guest");

    struct argparse_option *port_opt = argparse_add_option(
        parser, 'p', "port", ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT,
        "Port number", "port");
    argparse_option_set_default(port_opt, "80");

    struct argparse_option *rate_opt = argparse_add_option(
        parser, 'r', "rate", ARGPARSE_NARGS_1, ARGPARSE_TYPE_FLOAT,
        "Rate", "rate");
    argparse_option_set_default(rate_opt, "1.0");

    struct argparse_option *size_opt = argparse_add_option(
        parser, 's', "size", ARGPARSE_NARGS_1, ARGPARSE_TYPE_DOUBLE,
        "Size in MB", "size");
    argparse_option_set_default(size_opt, "256.0");

    struct argparse_option *count_opt = argparse_add_option(
        parser, 'c', "count", ARGPARSE_NARGS_1, ARGPARSE_TYPE_UINT,
        "Count", "count");
    argparse_option_set_default(count_opt, "10");

    struct argparse_option *verbose_opt = argparse_add_option(
        parser, 'v', "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE,
        "Verbose output", NULL);
    argparse_option_set_action(verbose_opt, ARGPARSE_STORE_TRUE);

    struct argparse_option *color_opt = argparse_add_option(
        parser, 0, "color", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Color", "color");
    argparse_option_set_default(color_opt, "auto");

    const char *argv[] = {"options", "--name", "Alice", "--port", "8080",
                          "--rate", "3.14", "--verbose"};
    struct argparse_result *result = argparse_parse(parser, 7, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    printf("name:    %s\n", argparse_get_string(result, "name"));
    printf("port:    %d\n", argparse_get_int(result, "port"));
    printf("rate:    %.2f\n", argparse_get_float(result, "rate"));
    printf("verbose: %s\n", argparse_get_bool(result, "verbose") ? "yes" : "no");

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```
