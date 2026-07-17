---
title: Help Formatting
description: Customizing help output and formatting
---

# Help Formatting

argparse-c generates beautiful help text automatically. You can customize it to match your needs.

## Default Help Output

```
Usage: myapp [OPTIONS] INPUT

A sample application

Options:
  -v, --verbose       Enable verbose output
  -o, --output FILE   Output file
  -n, --count N       Number of iterations
  -h, --help          Show this help message
  -V, --version       Show version
```

## Customizing Help

### Program Description

```c
struct argparse *parser = argparse_new(
    "myapp",
    "A powerful file processing tool\n"
    "Supports multiple formats and transformations"
);
```

### Epilog

```c
argparse_set_epilog(parser,
    "Examples:\n"
    "  myapp -v input.txt\n"
    "  myapp --count 5 -o output.txt input.txt\n"
    "  myapp --format json input.csv"
);
```

### Version String

```c
argparse_set_version(parser, "1.2.3");
```

## Help Formatting Options

### Custom Format

```c
argparse_set_usage_format(parser,
    "Usage: %prog [OPTIONS] FILE..."
);
```

### Positional Section Title

```c
argparse_set_positional_title(parser, "Files");
```

### Options Section Title

```c
argparse_set_options_title(parser, "Available Options");
```

## Example: Formatted CLI

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    struct argparse *parser = argparse_new(
        "convert",
        "File format converter\n"
        "Supports CSV, JSON, XML, and YAML formats"
    );

    argparse_set_version(parser, "2.0.0");
    argparse_set_epilog(parser,
        "Examples:\n"
        "  convert input.csv -o output.json\n"
        "  convert --format xml input.json\n"
        "  convert -v input.csv output.json"
    );

    argparse_add_option(parser, 'f', "format", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Output format", "FMT");
    argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Output file", "FILE");
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose output", NULL);

    argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                            "Input file", "INPUT");

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

    /* Process file */

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Help Output

```
Usage: convert [OPTIONS] INPUT

File format converter
Supports CSV, JSON, XML, and YAML formats

Options:
  -f, --format FMT   Output format
  -o, --output FILE  Output file
  -v, --verbose      Verbose output
  -h, --help         Show this help message
  -V, --version      Show version

Examples:
  convert input.csv -o output.json
  convert --format xml input.json
  convert -v input.csv output.json
```

## Next Steps

- [Error Handling](./error-handling) - Handling parse errors
- [API: Formatting](../api/format) - Complete API reference
