---
title: Arguments
description: Understanding argument types and configuration
---

# Arguments

argparse-c supports two types of arguments: **options** (named flags) and **positionals** (positional arguments).

## Argument Types Overview

| Type | Example | Description |
|------|---------|-------------|
| Option | `--verbose`, `-v` | Named flags that modify behavior |
| Positional | `input.txt` | Values identified by position |

## Defining Arguments

### Options

Options are defined with short and long names:

```c
// Short only
argparse_add_option(parser, 'v', NULL, ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Verbose mode", NULL);

// Long only
argparse_add_option(parser, '\0', "verbose", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Verbose mode", NULL);

// Both
argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Verbose mode", NULL);
```

### Positional Arguments

Positional arguments are defined by their order:

```c
// Required (exactly one)
argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                        "Input file", "INPUT");

// Optional (zero or one)
argparse_add_positional(parser, ARGPARSE_NARGS_0, ARGPARSE_TYPE_STRING,
                        "Output file", "OUTPUT");

// Variable (zero or more)
argparse_add_positional(parser, ARGPARSE_NARGS_NONE, ARGPARSE_TYPE_STRING,
                        "Additional files", "FILES");
```

## Argument Properties

| Property | Description |
|----------|-------------|
| `dest` | Internal name for accessing the value |
| `help` | Description shown in help text |
| `type` | Data type of the argument |
| `nargs` | Number of values expected |
| `default` | Default value if not provided |
| `required` | Whether the argument is required |

## Reading Values

```c
struct argparse_result *result = argparse_parse(parser, argc, argv);

/* String values */
const char *name = argparse_get_string(result, "name");

/* Integer values */
int count = argparse_get_int(result, "count");

/* Boolean values */
int verbose = argparse_get_bool(result, "verbose");

/* Float values */
double ratio = argparse_get_float(result, "ratio");
```

## Argument Naming

### Dest Names

The `dest` parameter determines how you access the value:

```c
argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_STRING, "Output file", "output_file");

/* Access using dest name */
const char *file = argparse_get_string(result, "output_file");
```

### Dest Name Rules

- Must be unique across all arguments
- Used as the key for `argparse_get_*()` functions
- Automatically generated if not provided

## Next Steps

- [Options](./options) - Deep dive into option configuration
- [Positional](./positional) - Positional argument specifics
- [Types](./types) - Available data types
