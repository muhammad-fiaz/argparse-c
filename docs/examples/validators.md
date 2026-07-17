# Validators

Custom value validation for options.

## Range Validator

```c
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <stdlib.h>

bool validate_range(const char *value, char *error_buf, size_t error_buf_size) {
    int num = atoi(value);
    if (num < 1 || num > 100) {
        snprintf(error_buf, error_buf_size,
                 "Value must be between 1 and 100, got: %s", value);
        return false;
    }
    return true;
}

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("app", "Application");

    struct argparse_option *opt = argparse_add_option(
        parser, 'p', "port", ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT,
        "Port number (1-10000)", "port");
    argparse_option_set_validator(opt, validate_range);

    struct argparse_result *result = argparse_parse(parser, argc, argv);
    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    printf("Port: %d\n", argparse_get_int(result, "port"));

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

```bash
$ ./app --port 8080
Port: 8080

$ ./app --port 99999
Error: Value must be between 1 and 10000, got: 99999
```

## Email Validator

```c
bool validate_email(const char *value, char *error_buf, size_t error_buf_size) {
    const char *at = strchr(value, '@');
    if (!at) {
        snprintf(error_buf, error_buf_size, "Invalid email: %s", value);
        return false;
    }
    const char *dot = strrchr(at, '.');
    if (!dot || dot == at + 1) {
        snprintf(error_buf, error_buf_size, "Invalid email: %s", value);
        return false;
    }
    return true;
}

struct argparse_option *email_opt = argparse_add_option(
    parser, 'e', "email", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
    "Email address", "email");
argparse_option_set_validator(email_opt, validate_email);
```

## Multiple Validators

```c
bool validate_nonempty(const char *value, char *error_buf, size_t error_buf_size) {
    if (strlen(value) == 0) {
        snprintf(error_buf, error_buf_size, "Value cannot be empty");
        return false;
    }
    return true;
}

bool validate_alphanumeric(const char *value, char *error_buf, size_t error_buf_size) {
    for (const char *p = value; *p; p++) {
        if (!isalnum((unsigned char)*p) && *p != '_' && *p != '-') {
            snprintf(error_buf, error_buf_size,
                     "Invalid character in name: '%c'", *p);
            return false;
        }
    }
    return true;
}
```

## Notes

- Validators run after type conversion and choice checking
- Return `true` for valid, `false` for invalid
- Write a descriptive error message to `error_buf` on failure
- The error buffer should be at least 256 bytes
