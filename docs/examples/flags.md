---
title: "Flags & Switches"
description: "Examples for boolean flags, switches, and counter options."
---

# Flags & Switches

Boolean flags and counter options.

## Boolean Flags

### Store True / Store False

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("app", "Application");

    /* --verbose sets verbose to true */
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Enable verbose output", "verbose");

    /* --no-color sets color to false */
    argparse_add_option(parser, 0, "no-color", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Disable colors", "color");

    /* --debug enables debug mode */
    argparse_add_option(parser, 'd', "debug", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Debug mode", "debug");

    struct argparse_result *result = argparse_parse(parser, argc, argv);

    bool verbose = argparse_get_bool(result, "verbose");
    bool color = argparse_get_bool(result, "color");
    bool debug = argparse_get_bool(result, "debug");

    printf("Verbose: %s\n", verbose ? "on" : "off");
    printf("Color: %s\n", color ? "on" : "off");
    printf("Debug: %s\n", debug ? "on" : "off");

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

```bash
$ ./app --verbose --no-color
Verbose: on
Color: off
Debug: off
```

## Counter Options

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("app", "Application");

    struct argparse_option *verbose_opt = argparse_add_option(
        parser, 'v', "verbose", ARGPARSE_NARGS_0,
        ARGPARSE_TYPE_NONE, "Verbosity level (repeat for more)", "verbose");
    argparse_option_set_action(verbose_opt, ARGPARSE_COUNT);

    struct argparse_result *result = argparse_parse(parser, argc, argv);

    int verbosity = argparse_get_int(result, "verbose");
    printf("Verbosity level: %d\n", verbosity);

    if (verbosity >= 3) {
        printf("Maximum verbosity\n");
    } else if (verbosity >= 2) {
        printf("High verbosity\n");
    } else if (verbosity >= 1) {
        printf("Normal verbosity\n");
    } else {
        printf("Quiet mode\n");
    }

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

```bash
$ ./app
Verbosity level: 0
Quiet mode

$ ./app -v
Verbosity level: 1
Normal verbosity

$ ./app -vv
Verbosity level: 2
High verbosity

$ ./app -vvv
Verbosity level: 3
Maximum verbosity
```

## Combining Flags

```c
struct argparse *parser = argparse_new("git", "Version control");

argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Verbose output", "verbose");
argparse_add_option(parser, 'q', "quiet", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Quiet mode", "quiet");
argparse_add_option(parser, 'n', "dry-run", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Dry run", "dry_run");
argparse_add_option(parser, 'f', "force", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Force operation", "force");
```
