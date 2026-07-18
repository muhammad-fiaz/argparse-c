---
title: "Formatting API"
description: "Help, usage, and version output formatting APIs."
---

# Formatting API

Help output and version information formatting.

## Functions

### `argparse_print_help`

```c
void argparse_print_help(const struct argparse *parser);
```

Print full help information to stdout, including:
- Program name and usage line
- Description
- Option groups with their options
- Epilog

### `argparse_print_usage`

```c
void argparse_print_usage(const struct argparse *parser);
```

Print a single-line usage summary to stdout.

### `argparse_print_version`

```c
void argparse_print_version(const struct argparse *parser);
```

Print version information to stdout.

## Example

```c
struct argparse *parser = argparse_new("myapp", "My application");
argparse_set_epilog(parser, "Examples:\n  myapp -o file.txt input.dat");

/* Add options... */

/* Print help manually */
argparse_print_help(parser);
```

Output:

```
usage: myapp [-h] [-o OUTPUT] [-v] input

My application

positional arguments:
  input              Input file

options:
  -h, --help         Show this help message and exit
  -o, --output FILE  Output file
  -v, --verbose      Verbose output

Examples:
  myapp -o file.txt input.dat
```

## Notes

- Output is written to stdout
- Output is formatted to fit within 80 columns
- Unicode characters are correctly aligned using display width calculation
- Help is automatically generated for the `--help` option
