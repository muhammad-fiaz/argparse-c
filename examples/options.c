/*
 * examples/options.c - All option types with defaults
 *
 * Demonstrates STRING, INT, UINT, FLOAT, DOUBLE, BOOL, and ENUM types
 * with custom default values.
 *
 * Run:
 *   ./options --name Alice --port 8080 --rate 3.14 --color blue
 */

#include <stdio.h>
#include <argparse-c/argparse.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("options", "All option types demo");

    /* String option with default */
    struct argparse_option *name = argparse_add_option(parser, 'n', "name",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Your name", "name");
    argparse_option_set_default(name, "World");

    /* Integer option with default */
    struct argparse_option *port = argparse_add_option(parser, 'p', "port",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Port number", "port");
    argparse_option_set_default(port, "80");

    /* Unsigned integer option */
    argparse_add_option(parser, 's', "size",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_UINT, "Buffer size", "size");

    /* Float option */
    argparse_add_option(parser, 'r', "rate",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_FLOAT, "Processing rate", "rate");

    /* Double option */
    argparse_add_option(parser, 'P', "precision",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_DOUBLE, "Precision level", "precision");

    /* Boolean option */
    argparse_add_option(parser, 'f', "force",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_BOOL, "Force operation", "force");

    /* Enum option */
    struct argparse_option *color = argparse_add_option(parser, 'c', "color",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_ENUM, "Color scheme", "color");
    const char *colors[] = {"red", "green", "blue", "auto"};
    argparse_option_set_choices(color, colors, 4);
    argparse_option_set_default(color, "auto");

    struct argparse_result *result = argparse_parse(parser, argc, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    printf("name:      %s\n", argparse_get_string(result, "name"));
    printf("port:      %d\n", argparse_get_int(result, "port"));
    printf("size:      %u\n", argparse_get_uint(result, "size"));
    printf("rate:      %.2f\n", argparse_get_float(result, "rate"));
    printf("precision: %.6f\n", argparse_get_double(result, "precision"));
    printf("force:     %s\n", argparse_get_bool(result, "force") ? "true" : "false");
    printf("color:     %s\n", argparse_get_enum(result, "color"));

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
