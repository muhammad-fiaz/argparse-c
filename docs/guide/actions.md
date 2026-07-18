---
title: Actions
description: Custom actions for options
---

# Actions

Actions let you customize what happens when an option is encountered during parsing.

## Built-in Actions

### Store (Default)

The default action stores the value:

```c
argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_STRING, "Output file", "FILE");
// Stores the value provided
```

### Store True

Sets the flag to true when encountered:

```c
argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Verbose mode", NULL);
// Sets verbose to 1 when -v is used
```

### Store False

Sets the flag to false when encountered:

```c
argparse_add_option(parser, 'q', "quiet", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Quiet mode", NULL);
// Use with custom logic to set verbose to 0
```

### Count

Increments a counter each time the option is used:

```c
// For repeated flags like -vvv
argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Increase verbosity", NULL);
// -v = 1, -vv = 2, -vvv = 3
```

## Custom Actions

### Append Action

Collect multiple values into a list:

```c
argparse_add_option(parser, 'I', "include", ARGPARSE_NARGS_STAR,
                    ARGPARSE_TYPE_STRING, "Include paths", "PATH");

// Usage: prog -I path1 -I path2 -I path3
// Result: include = ["path1", "path2", "path3"]
```

### Const Action

Use a constant value when the option is present:

```c
argparse_add_option(parser, 'd', "debug", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Debug mode", NULL);
// Sets a constant value when -d is used
```

## Example: Verbosity Levels

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("app", "Application with verbosity");

    /* -v can be used multiple times */
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Increase verbosity (use multiple times)", NULL);

    argparse_add_option(parser, 'q', "quiet", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Quiet mode", NULL);

    struct argparse_result *result = argparse_parse(
        parser, argc, argv
    );

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    if (result == NULL || argparse_result_error_code(result) != ARGPARSE_ERROR_UNKNOWN) {
        argparse_result_free(result);
        argparse_free(parser);
        return 0;
    }

    int verbosity = argparse_get_int(result, "verbose");
    int quiet = argparse_get_bool(result, "quiet");

    if (quiet) {
        verbosity = 0;
    }

    if (verbosity >= 3) {
        printf("DEBUG: Very detailed output\n");
    }
    if (verbosity >= 2) {
        printf("INFO: Detailed output\n");
    }
    if (verbosity >= 1) {
        printf("INFO: Basic output\n");
    }

    printf("Verbosity level: %d\n", verbosity);

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Next Steps

- [Validators](./validators) - Custom validation logic
- [Groups](./groups) - Grouping related arguments
- [API: Actions](../api/actions) - Complete API reference
