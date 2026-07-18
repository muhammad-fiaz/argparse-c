---
title: "Config Files"
description: "Loading arguments from JSON, INI, YAML, and TOML config files"
---

# Config Files

argparse-c supports loading option values from configuration files, providing a convenient way to set defaults without command-line flags.

## Overview

Configuration files allow users to persist commonly-used options. The precedence order is:

1. **Command-line arguments** (highest priority)
2. **Environment variables**
3. **Configuration files** (lowest priority)

## Loading Config Files

```c
argparse_error_code err = argparse_load_config(parser, "config.ini");
if (err != ARGPARSE_OK) {
    fprintf(stderr, "Failed to load config: %s\n", argparse_error_string(err));
}
```

## Supported Formats

### INI Format

```ini
# This is a comment
[general]
verbose = true
output = result.txt
count = 42

[advanced]
log-level = debug
timeout = 30
```

### Key-Value Format

```
# Lines starting with # are comments
verbose = true
output = result.txt
count = 42
log-level = debug
```

### Boolean Values

Boolean options accept:

- `true`, `false`
- `yes`, `no`
- `on`, `off`
- `1`, `0`

## Example

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("myapp", "Application with config");

    argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Output file", "output");
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose mode", "verbose");
    argparse_add_option(parser, 'n', "count", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Repeat count", "count");

    /* Load config file (before parsing) */
    argparse_load_config(parser, ".myapp.conf");

    /* Parse command line (overrides config file values) */
    struct argparse_result *result = argparse_parse(parser, argc, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    const char *output = argparse_get_string(result, "output");
    bool verbose = argparse_get_bool(result, "verbose");
    int count = argparse_get_int(result, "count");

    printf("Output: %s\n", output ? output : "(none)");
    printf("Verbose: %s\n", verbose ? "yes" : "no");
    printf("Count: %d\n", count);

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Config File Search

argparse-c does not automatically search for config files. You should implement the search pattern appropriate for your application:

```c
void load_config(struct argparse *parser) {
    /* Check current directory */
    if (argparse_load_config(parser, ".myapp.conf") == ARGPARSE_OK) {
        return;
    }

    /* Check home directory */
    const char *home = getenv("HOME");
    if (home) {
        char path[1024];
        snprintf(path, sizeof(path), "%s/.myapp.conf", home);
        argparse_load_config(parser, path);
    }
}
```

## Notes

- Config file values are treated as if they appeared on the command line
- Command-line arguments always override config file values
- Environment variables (when configured) override config file values
- If the config file doesn't exist, `argparse_load_config` returns an error (which you can safely ignore)
