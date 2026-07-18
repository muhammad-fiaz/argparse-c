---
title: "Errors API"
description: "Error handling and error string utilities. typedef enum argparse_error_code { const char *argparse_error_string(argparse_error_code code); Convert an error code to a human-readable string. const char *argparse_result_error(const struct argparse_result *result); Get the detailed error message from a parse result. argparse_error_code argparse_result_error_code(const struct argparse_result *result); Get the error code from a parse result. struct argparse_result *result = argparse_parse(parser, argc, argv); argparse_error_code code = argparse_result_error_code(result); if (code != ARGPARSE_OK) { argparse_result_free(result); struct argparse_result *result = argparse_parse(parser, argc, argv); switch (argparse_result_error_code(result)) { case ARGPARSE_OK: case ARGPARSE_ERROR_MISSING_ARGUMENT: case ARGPARSE_ERROR_INVALID_VALUE: case ARGPARSE_ERROR_CONFLICT: default: argparse_result_free(result);"
---

# Errors API

Error handling and error string utilities.

## Error Codes

```c
typedef enum argparse_error_code {
    ARGPARSE_OK = 0,
    ARGPARSE_ERROR_UNKNOWN,
    ARGPARSE_ERROR_MISSING_ARGUMENT,
    ARGPARSE_ERROR_UNEXPECTED_ARGUMENT,
    ARGPARSE_ERROR_INVALID_VALUE,
    ARGPARSE_ERROR_TOO_MANY_ARGUMENTS,
    ARGPARSE_ERROR_AMBIGUOUS_OPTION,
    ARGPARSE_ERROR_CONFLICT,
    ARGPARSE_ERROR_OUT_OF_MEMORY,
    ARGPARSE_ERROR_INVALID_CONFIG
} argparse_error_code;
```

## Functions

### `argparse_error_string`

```c
const char *argparse_error_string(argparse_error_code code);
```

Convert an error code to a human-readable string.

**Returns:** Static string. Must not be freed.

### `argparse_result_error`

```c
const char *argparse_result_error(const struct argparse_result *result);
```

Get the detailed error message from a parse result.

**Returns:** Error message string, or NULL if no error occurred.

### `argparse_result_error_code`

```c
argparse_error_code argparse_result_error_code(const struct argparse_result *result);
```

Get the error code from a parse result.

## Example

```c
struct argparse_result *result = argparse_parse(parser, argc, argv);
argparse_error_code code = argparse_result_error_code(result);

if (code != ARGPARSE_OK) {
    fprintf(stderr, "Error: %s\n", argparse_result_error(result));
    fprintf(stderr, "Error code: %s\n", argparse_error_string(code));
}

argparse_result_free(result);
```

## Error Handling Pattern

```c
struct argparse_result *result = argparse_parse(parser, argc, argv);

switch (argparse_result_error_code(result)) {
case ARGPARSE_OK:
    /* Success */
    break;

case ARGPARSE_ERROR_MISSING_ARGUMENT:
    fprintf(stderr, "Missing argument: %s\n", argparse_result_error(result));
    argparse_print_usage(parser);
    break;

case ARGPARSE_ERROR_INVALID_VALUE:
    fprintf(stderr, "Invalid value: %s\n", argparse_result_error(result));
    break;

case ARGPARSE_ERROR_CONFLICT:
    fprintf(stderr, "Conflicting options: %s\n", argparse_result_error(result));
    break;

default:
    fprintf(stderr, "Parse error: %s\n", argparse_result_error(result));
    break;
}

argparse_result_free(result);
```

## Notes

- Error strings from `argparse_error_string` are static and thread-safe
- Error messages from `argparse_result_error` are owned by the result
- Always free the result even on error
