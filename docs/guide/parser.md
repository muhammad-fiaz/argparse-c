---
title: Parser
description: Configuring and using the argparse parser
---

# Parser

The parser is the core object in argparse-c. It manages argument definitions and parsing logic.

## Creating a Parser

```c
struct argparse *parser = argparse_new("myapp", "My application description");
```

### Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `prog` | `const char *` | Program name displayed in help |
| `description` | `const char *` | Short description of the program |

### Memory

The parser allocates memory on the heap. Always free it when done:

```c
argparse_free(parser);
```

## Adding Arguments

### Options

```c
// Boolean flag
argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Enable verbose mode", NULL);

// String option
argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_STRING, "Output file", "FILE");

// Integer option
argparse_add_option(parser, 'n', "count", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_INT, "Number of iterations", "N");
```

### Positional Arguments

```c
// Required positional
argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                        "Input file path", "INPUT");

// Optional positional
argparse_add_positional(parser, ARGPARSE_NARGS_0, ARGPARSE_TYPE_STRING,
                        "Output file (default: stdout)", "OUTPUT");
```

## Parsing

```c
struct argparse_result *result = argparse_parse(
    parser, argc, (const char **)argv
);
```

### Return Value

The result object contains:

- **Parsed values** - Access via `argparse_get_*()` functions
- **Error information** - Check with `argparse_result_error_code()`
- **Exit flag** - Check with `argparse_result_should_exit()`

### Error Handling

```c
if (argparse_result_error_code(result) != ARGPARSE_OK) {
    fprintf(stderr, "Parse error: %s\n", argparse_result_error(result));
    argparse_result_free(result);
    argparse_free(parser);
    return 1;
}

if (argparse_result_should_exit(result)) {
    // --help or --version was invoked
    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Parser Options

### Custom Help Text

```c
argparse_set_epilog(parser, "Examples:\n  myapp -v input.txt\n  myapp --count 5");
```

### Version String

```c
argparse_set_version(parser, "1.0.0");
```

### Prefix Character

Change the option prefix from `-` to `/`:

```c
argparse_set_prefix(parser, '/');
```

## Example: Complete Parser Setup

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    /* Create parser */
    struct argparse *parser = argparse_new(
        "converter",
        "File format converter"
    );

    /* Configure parser */
    argparse_set_version(parser, "2.1.0");
    argparse_set_epilog(parser,
        "Examples:\n"
        "  converter input.csv -o output.json\n"
        "  converter --format xml input.txt"
    );

    /* Add options */
    argparse_add_option(parser, 'f', "format", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Output format", "FMT");
    argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Output file", "FILE");
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose output", NULL);
    argparse_add_option(parser, 'q', "quiet", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Suppress output", NULL);

    /* Add positional */
    argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                            "Input file", "INPUT");

    /* Parse */
    struct argparse_result *result = argparse_parse(
        parser, argc, (const char **)argv
    );

    /* Handle results */
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

    /* Use parsed values */
    const char *input = argparse_get_string(result, "INPUT");
    const char *format = argparse_get_string(result, "format");
    const char *output = argparse_get_string(result, "output");
    int verbose = argparse_get_bool(result, "verbose");

    printf("Converting %s to %s format\n", input, format ? format : "auto");

    /* Cleanup */
    argparse_result_free(result);
    argparse_free(parser);

    return 0;
}
```

## Next Steps

- [Arguments](./arguments) - Learn about argument types and options
- [Options](./options) - Deep dive into option configuration
- [API: Parser](../api/parser) - Complete API reference
