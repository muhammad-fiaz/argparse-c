# Validators API

Custom value validation for options.

## Types

### `argparse_validate_fn`

```c
typedef bool (*argparse_validate_fn)(const char *value,
                                     char *error_buf,
                                     size_t error_buf_size);
```

Custom validation function type. Called after type conversion.

**Parameters:**
- `value` — The string value to validate
- `error_buf` — Buffer for error message on failure
- `error_buf_size` — Size of error buffer

**Returns:** true if valid, false if validation failed.

## Functions

### `argparse_option_set_validator`

```c
void argparse_option_set_validator(struct argparse_option *option,
                                   argparse_validate_fn validator);
```

Set a custom validator on an option.

## Example

```c
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <stdlib.h>

bool validate_port(const char *value, char *error_buf, size_t error_buf_size) {
    int port = atoi(value);
    if (port < 1 || port > 65535) {
        snprintf(error_buf, error_buf_size,
                 "Port must be between 1 and 65535, got: %s", value);
        return false;
    }
    return true;
}

bool validate_email(const char *value, char *error_buf, size_t error_buf_size) {
    const char *at = strchr(value, '@');
    if (!at || !strchr(at + 1, '.')) {
        snprintf(error_buf, error_buf_size,
                 "Invalid email address: %s", value);
        return false;
    }
    return true;
}

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("app", "Application");

    struct argparse_option *port_opt = argparse_add_option(
        parser, 'p', "port", ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT,
        "Port number", "port");
    argparse_option_set_validator(port_opt, validate_port);

    struct argparse_option *email_opt = argparse_add_option(
        parser, 'e', "email", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Email address", "email");
    argparse_option_set_validator(email_opt, validate_email);

    struct argparse_result *result = argparse_parse(parser, argc, argv);
    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
    }

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Notes

- Validators run after type conversion and choice checking
- If the validator returns false, parsing is aborted with the error message
- The error buffer should be at least 256 bytes
- Validators are called once per option, not per value in multi-value options
