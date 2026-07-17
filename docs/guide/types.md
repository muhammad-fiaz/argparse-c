---
title: Types
description: Available data types and type conversion
---

# Types

argparse-c supports several built-in types with automatic conversion and validation.

## Built-in Types

| Type | C Type | Example | Description |
|------|--------|---------|-------------|
| `ARGPARSE_TYPE_NONE` | `int` | `-v` | Boolean flag (no value) |
| `ARGPARSE_TYPE_STRING` | `const char *` | `"hello"` | String value |
| `ARGPARSE_TYPE_INT` | `int` | `42` | Integer value |
| `ARGPARSE_TYPE_FLOAT` | `double` | `3.14` | Floating-point value |
| `ARGPARSE_TYPE_BOOL` | `int` | `true` | Boolean value |

## Using Types

### String

```c
argparse_add_option(parser, 'n', "name", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_STRING, "Your name", "NAME");

// Access
const char *name = argparse_get_string(result, "name");
```

### Integer

```c
argparse_add_option(parser, 'c', "count", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_INT, "Number of iterations", "N");

// Access
int count = argparse_get_int(result, "count");
```

### Float

```c
argparse_add_option(parser, 'r', "ratio", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_FLOAT, "Scaling ratio", "RATIO");

// Access
double ratio = argparse_get_float(result, "ratio");
```

### Boolean

```c
argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Verbose mode", NULL);

// Access
int verbose = argparse_get_bool(result, "verbose");
```

## Type Conversion

argparse-c automatically converts string input to the target type:

```c
// User types: prog --count 42
argparse_add_option(parser, 'c', "count", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_INT, "Count", "N");
// Automatically converts "42" to int 42
```

### Conversion Errors

```c
// User types: prog --count abc
// Result: ARGPARSE_ERR_INVALID_VALUE
// Error message: "Invalid integer value: 'abc'"
```

## Type Validation

### Integer Validation

```c
// Validates that input is a valid integer
// Rejects: "abc", "12.34", ""
// Accepts: "42", "-5", "0", "1000000"
```

### Float Validation

```c
// Validates that input is a valid float
// Rejects: "abc", ""
// Accepts: "3.14", "-1.5", "1e10", "inf", "nan"
```

## Custom Types

For types not covered by built-in support, use strings and convert manually:

```c
argparse_add_option(parser, 'd', "date", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_STRING, "Date (YYYY-MM-DD)", "DATE");

// Manual conversion
const char *date_str = argparse_get_string(result, "date");
int year, month, day;
if (sscanf(date_str, "%d-%d-%d", &year, &month, &day) != 3) {
    fprintf(stderr, "Invalid date format\n");
    return 1;
}
```

## Example: Type Demo

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    struct argparse *parser = argparse_new("types", "Type demonstration");

    argparse_add_option(parser, 's', "string", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "A string value", "STR");
    argparse_add_option(parser, 'i', "int", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "An integer value", "N");
    argparse_add_option(parser, 'f', "float", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_FLOAT, "A float value", "F");
    argparse_add_option(parser, 'b', "bool", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "A boolean flag", NULL);

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

    printf("String: %s\n", argparse_get_string(result, "string"));
    printf("Int: %d\n", argparse_get_int(result, "int"));
    printf("Float: %f\n", argparse_get_float(result, "float"));
    printf("Bool: %s\n", argparse_get_bool(result, "bool") ? "true" : "false");

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Next Steps

- [Actions](./actions) - Custom option behavior
- [Validators](./validators) - Custom validation logic
- [API: Options](../api/options) - Complete API reference
