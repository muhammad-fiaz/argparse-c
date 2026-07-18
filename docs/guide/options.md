---
title: Options
description: Configuring command-line options and flags
---

# Options

Options are named flags that modify program behavior. They can be boolean switches or accept values.

## Option Types

### Boolean Flags

Flags that don't take a value:

```c
argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Enable verbose output", NULL);

// Usage: prog -v or prog --verbose
```

### Value Options

Options that accept a value:

```c
argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_STRING, "Output file", "FILE");

// Usage: prog -o file.txt or prog --output file.txt
```

### Multiple Values

Options that accept multiple values:

```c
argparse_add_option(parser, 'I', "include", ARGPARSE_NARGS_STAR,
                    ARGPARSE_TYPE_STRING, "Include paths", "PATH");

// Usage: prog -I path1 -I path2 -I path3
```

## Option Syntax

### Short Options

Single-character options prefixed with `-`:

```bash
prog -v
prog -o file.txt
prog -v -o file.txt
prog -vo file.txt  # Combined flags
```

### Long Options

Multi-character options prefixed with `--`:

```bash
prog --verbose
prog --output file.txt
prog --verbose --output file.txt
```

### Mixed Usage

```bash
prog -v --output file.txt
prog --verbose -o file.txt
```

## Option Configuration

### Complete Example

```c
argparse_add_option(
    parser,
    'o',           // short_name ('\0' for none)
    "output",      // long_name (NULL for none)
    ARGPARSE_NARGS_1,  // nargs
    ARGPARSE_TYPE_STRING,  // type
    "Output file path",    // help text
    "FILE"                 // metavar (shown in help)
);
```

### Configuration Table

| Parameter | Description | Example |
|-----------|-------------|---------|
| `short_name` | Single character | `'v'` |
| `long_name` | Long option name | `"verbose"` |
| `nargs` | Number of values | `ARGPARSE_NARGS_1` |
| `type` | Value type | `ARGPARSE_TYPE_STRING` |
| `help` | Help description | `"Enable verbose"` |
| `metavar` | Placeholder in help | `"FILE"` |

## Special Options

### Help Option

argparse-c automatically adds `--help` / `-h`:

```c
// Automatic help is added by default
// Users can invoke it with: prog --help
```

### Version Option

Add a version flag:

```c
/* Note: argparse_set_version() is not available in the API.
   Use argparse_set_prog() to set the program name, or handle version separately. */
```

## Advanced Options

### Default Values

```c
// Set default in the option definition
argparse_add_option(parser, 'n', "count", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_INT, "Number of iterations", "N");
// Default is 0 if not specified
```

### Required Options

```c
// Make an option required
struct argparse_option *opt = argparse_add_option(parser, 'f', "format", ARGPARSE_NARGS_1,
                             ARGPARSE_TYPE_STRING, "Output format", "FMT");
argparse_option_set_required(opt, true);
```

## Example: Complex Options

```c
#include <argparse-c/argparse.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("build", "Build system");

    argparse_add_option(parser, 'j', "jobs", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Number of parallel jobs", "N");
    argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Output directory", "DIR");
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose output", NULL);
    argparse_add_option(parser, 'D', "define", ARGPARSE_NARGS_STAR,
                        ARGPARSE_TYPE_STRING, "Preprocessor defines", "KEY=VAL");
    argparse_add_option(parser, 'I', "include", ARGPARSE_NARGS_STAR,
                        ARGPARSE_TYPE_STRING, "Include directories", "DIR");

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

    int jobs = argparse_get_int(result, "jobs");
    const char *output = argparse_get_string(result, "output");
    int verbose = argparse_get_bool(result, "verbose");

    printf("Building with %d jobs to %s\n", jobs, output ? output : ".");

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Next Steps

- [Positional](./positional) - Positional argument details
- [Types](./types) - Available data types
- [Actions](./actions) - Custom option behavior
