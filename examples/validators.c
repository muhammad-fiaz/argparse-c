/*
 * examples/validators.c - Custom validators
 *
 * Demonstrates attaching custom validation functions to options
 * that check values after type conversion.
 *
 * Run:
 *   ./validators --port 80          # OK
 *   ./validators --port 99999       # Error: port out of range
 *   ./validators --email bad        # Error: invalid email
 *   ./validators --email user@host.com  # OK
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <argparse-c/argparse.h>

static bool validate_port(const char *value, char *error_buf, size_t error_buf_size) {
    int port = atoi(value);
    if (port < 1 || port > 65535) {
        snprintf(error_buf, error_buf_size, "Port must be between 1 and 65535, got %d", port);
        return false;
    }
    return true;
}

static bool validate_email(const char *value, char *error_buf, size_t error_buf_size) {
    const char *at = strchr(value, '@');
    if (!at || at == value || !at[1]) {
        snprintf(error_buf, error_buf_size, "Invalid email format: '%s'", value);
        return false;
    }
    const char *dot = strrchr(at, '.');
    if (!dot || dot == at + 1 || !dot[1]) {
        snprintf(error_buf, error_buf_size, "Invalid email domain: '%s'", value);
        return false;
    }
    return true;
}

static bool validate_positive(const char *value, char *error_buf, size_t error_buf_size) {
    int num = atoi(value);
    if (num <= 0) {
        snprintf(error_buf, error_buf_size, "Value must be positive, got %d", num);
        return false;
    }
    return true;
}

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("validators", "Custom validation demo");

    /* Port with range validator */
    struct argparse_option *port = argparse_add_option(parser, 'p', "port",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Port number (1-65535)", "port");
    argparse_option_set_validator(port, validate_port);

    /* Email with format validator */
    struct argparse_option *email = argparse_add_option(parser, 'e', "email",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Contact email", "email");
    argparse_option_set_validator(email, validate_email);

    /* Count with positive validator */
    struct argparse_option *count = argparse_add_option(parser, 'n', "count",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Number of items", "count");
    argparse_option_set_validator(count, validate_positive);

    struct argparse_result *result = argparse_parse(parser, argc, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    printf("port:  %d\n", argparse_get_int(result, "port"));
    printf("email: %s\n", argparse_get_string(result, "email"));
    printf("count: %d\n", argparse_get_int(result, "count"));

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
