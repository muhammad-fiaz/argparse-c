---
title: Validators
description: Custom validation logic for arguments
---

# Validators

Validators let you add custom validation logic to arguments.

## Basic Validation

### Type Validation

argparse-c automatically validates types:

```c
argparse_add_option(parser, 'p', "port", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_INT, "Port number", "PORT");

// User types: prog --port abc
// Error: "Invalid integer value: 'abc'"
```

### Range Validation

For values that must be within a range:

```c
argparse_add_option(parser, 'p', "port", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_INT, "Port number (1-65535)", "PORT");

// Manual validation after parsing
int port = argparse_get_int(result, "port");
if (port < 1 || port > 65535) {
    fprintf(stderr, "Port must be between 1 and 65535\n");
    return 1;
}
```

## Custom Validators

### String Validation

```c
/* Validate string format */
const char *date = argparse_get_string(result, "date");
int year, month, day;
if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) {
    fprintf(stderr, "Invalid date format. Use YYYY-MM-DD\n");
    return 1;
}
```

### Email Validation

```c
const char *email = argparse_get_string(result, "email");
if (strchr(email, '@') == NULL) {
    fprintf(stderr, "Invalid email address\n");
    return 1;
}
```

### File Validation

```c
const char *path = argparse_get_string(result, "file");
FILE *f = fopen(path, "r");
if (f == NULL) {
    fprintf(stderr, "Cannot open file: %s\n", path);
    return 1;
}
fclose(f);
```

## Example: Comprehensive Validation

```c
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <string.h>

int validate_port(int port) {
    return port >= 1 && port <= 65535;
}

int validate_email(const char *email) {
    const char *at = strchr(email, '@');
    if (!at) return 0;
    const char *dot = strrchr(at, '.');
    return dot && dot > at + 1;
}

int main(int argc, char *argv[]) {
    struct argparse *parser = argparse_new("server", "Server application");

    argparse_add_option(parser, 'p', "port", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Port number (1-65535)", "PORT");
    argparse_add_option(parser, 'H', "host", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Host address", "HOST");
    argparse_add_option(parser, 'e', "email", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Admin email", "EMAIL");

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

    /* Validate port */
    int port = argparse_get_int(result, "port");
    if (!validate_port(port)) {
        fprintf(stderr, "Error: Port must be between 1 and 65535\n");
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    /* Validate email */
    const char *email = argparse_get_string(result, "email");
    if (email && !validate_email(email)) {
        fprintf(stderr, "Error: Invalid email address\n");
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    printf("Server starting on %s:%d\n",
           argparse_get_string(result, "host"), port);

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Validation Best Practices

1. **Validate early** - Check values immediately after parsing
2. **Provide clear errors** - Tell users exactly what's wrong
3. **Use ranges** - Define valid ranges for numeric values
4. **Check file access** - Verify files exist before using them
5. **Document constraints** - Include constraints in help text

## Next Steps

- [Groups](./groups) - Grouping related arguments
- [Error Handling](./error-handling) - Handling parse errors
- [API: Validators](../api/validators) - Complete API reference
