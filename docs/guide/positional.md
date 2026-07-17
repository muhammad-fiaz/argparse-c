---
title: Positional Arguments
description: Working with positional arguments
---

# Positional Arguments

Positional arguments are identified by their position on the command line rather than by name.

## Basic Usage

```c
// A required string argument
argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                        "Input file path", "INPUT");

// Usage: prog input.txt
```

## Nargs Configuration

### ARGPARSE_NARGS_1 - Exactly One

```c
argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                        "Input file", "INPUT");

// Usage: prog input.txt ✓
// Usage: prog            ✗ (error)
// Usage: prog a b        ✗ (error)
```

### ARGPARSE_NARGS_0 - Zero or One

```c
argparse_add_positional(parser, ARGPARSE_NARGS_0, ARGPARSE_TYPE_STRING,
                        "Output file (default: stdout)", "OUTPUT");

// Usage: prog           ✓ (optional)
// Usage: prog output.txt ✓
// Usage: prog a b        ✗ (error)
```

### ARGPARSE_NARGS_NONE - Zero or More

```c
argparse_add_positional(parser, ARGPARSE_NARGS_NONE, ARGPARSE_TYPE_STRING,
                        "Additional files", "FILES");

// Usage: prog              ✓ (empty list)
// Usage: prog file1        ✓
// Usage: prog file1 file2  ✓
// Usage: prog file1 file2 file3 ✓
```

## Position Matters

Positional arguments are matched in the order they are defined:

```c
argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                        "Source file", "SOURCE");
argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                        "Destination file", "DEST");

// Usage: prog src.txt dest.txt
//   SOURCE = "src.txt"
//   DEST = "dest.txt"
```

## Mixed with Options

Options can appear before, after, or between positional arguments:

```c
argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Verbose mode", NULL);
argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                        "Input file", "INPUT");

// All of these work:
// prog -v input.txt
// prog input.txt -v
// prog input.txt --verbose
```

## Example: File Processor

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    struct argparse *parser = argparse_new("processor", "Process files");

    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose output", NULL);
    argparse_add_option(parser, 'f', "force", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Overwrite existing files", NULL);

    argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                            "Source file", "SOURCE");
    argparse_add_positional(parser, ARGPARSE_NARGS_0, ARGPARSE_TYPE_STRING,
                            "Destination file (default: stdout)", "DEST");

    struct argparse_result *result = argparse_parse(
        parser, argc, (const char **)argv
    );

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    if (argparse_result_should_exit(result)) {
        argparse_result_free(result);
        argparse_free(parser);
        return 0;
    }

    const char *source = argparse_get_string(result, "SOURCE");
    const char *dest = argparse_get_string(result, "DEST");
    int verbose = argparse_get_bool(result, "verbose");

    if (verbose) {
        fprintf(stderr, "Processing %s\n", source);
    }

    if (dest) {
        printf("Copying %s to %s\n", source, dest);
    } else {
        printf("Output from %s\n", source);
    }

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Next Steps

- [Types](./types) - Available data types
- [Validators](./validators) - Custom validation logic
- [API: Options](../api/options) - Complete API reference
