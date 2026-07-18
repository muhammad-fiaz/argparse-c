---
title: Error Handling
description: Handling parsing errors gracefully
---

# Error Handling

argparse-c provides comprehensive error handling for argument parsing.

## Error Types

| Error Code | Description |
|------------|-------------|
| `ARGPARSE_OK` | Parsing succeeded |
| `ARGPARSE_ERROR_UNKNOWN` | Unknown error |
| `ARGPARSE_ERROR_MISSING_ARGUMENT` | Required argument not provided |
| `ARGPARSE_ERROR_INVALID_VALUE` | Value conversion failed |
| `ARGPARSE_ERROR_TOO_MANY_ARGUMENTS` | Too many arguments |
| `ARGPARSE_ERROR_AMBIGUOUS_OPTION` | Ambiguous option |
| `ARGPARSE_ERROR_CONFLICT` | Conflicting options |
| `ARGPARSE_ERROR_OUT_OF_MEMORY` | Out of memory |
| `ARGPARSE_ERROR_INVALID_CONFIG` | Invalid configuration |

## Basic Error Handling

```c
struct argparse_result *result = argparse_parse(
    parser, argc, argv
);

if (argparse_result_error_code(result) != ARGPARSE_OK) {
    fprintf(stderr, "Error: %s\n", argparse_result_error(result));
    argparse_result_free(result);
    argparse_free(parser);
    return 1;
}
```

## Error Messages

### Automatic Error Messages

argparse-c generates descriptive error messages:

```
Error: unrecognized option '--foo'
Error: option '--output' requires an argument
Error: invalid integer value: 'abc'
Error: expected 1 argument, got 2
```

### Custom Error Context

```c
if (argparse_result_error_code(result) != ARGPARSE_OK) {
    fprintf(stderr, "Error: %s\n", argparse_result_error(result));
    fprintf(stderr, "Run '--help' for usage information\n");
}
```

## Exit Handling

### Help and Version

```c
if (result == NULL || argparse_result_error_code(result) != ARGPARSE_ERROR_UNKNOWN) {
    /* --help or --version was invoked */
    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

### Programmatic Exit

```c
if (result == NULL || argparse_result_error_code(result) != ARGPARSE_ERROR_UNKNOWN) {
    int exit_code = argparse_result_error_code(result);
    argparse_result_free(result);
    argparse_free(parser);
    return exit_code;
}
```

## Example: Complete Error Handling

```c
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("server", "HTTP server");

    argparse_add_option(parser, 'p', "port", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Port number", "PORT");
    argparse_add_option(parser, 'H', "host", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Host address", "HOST");

    struct argparse_result *result = argparse_parse(
        parser, argc, argv
    );

    /* Handle parse errors */
    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        fprintf(stderr, "Try '--help' for more information\n");
        argparse_result_free(result);
        argparse_free(parser);
        return EXIT_FAILURE;
    }

    /* Handle --help/--version */
    if (result == NULL || argparse_result_error_code(result) != ARGPARSE_ERROR_UNKNOWN) {
        int code = argparse_result_error_code(result);
        argparse_result_free(result);
        argparse_free(parser);
        return code;
    }

    /* Validate port range */
    int port = argparse_get_int(result, "port");
    if (port < 1 || port > 65535) {
        fprintf(stderr, "Error: Port must be between 1 and 65535\n");
        argparse_result_free(result);
        argparse_free(parser);
        return EXIT_FAILURE;
    }

    printf("Starting server on %s:%d\n",
           argparse_get_string(result, "host"), port);

    argparse_result_free(result);
    argparse_free(parser);
    return EXIT_SUCCESS;
}
```

## Error Handling Best Practices

1. **Always check for errors** - Never assume parsing succeeded
2. **Provide helpful messages** - Tell users how to fix the issue
3. **Show usage hints** - Point users to `--help`
4. **Validate after parsing** - Check value ranges and constraints
5. **Clean up resources** - Free result and parser on all paths

## Next Steps

- [Memory Management](./memory) - Proper cleanup patterns
- [API: Errors](../api/errors) - Complete API reference
