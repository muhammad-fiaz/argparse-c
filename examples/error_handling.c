/*
 * examples/error_handling.c - Error diagnostics
 *
 * Demonstrates comprehensive error handling with detailed
 * diagnostics for various error conditions.
 *
 * Run:
 *   ./error_handling --port abc        # Invalid value
 *   ./error_handling --port 80 --port 443  # Too many values
 *   ./error_handling                  # Missing required option
 *   ./error_handling --unknown-flag   # Unknown option
 */

#include <stdio.h>
#include <argparse-c/argparse.h>

static const char *error_code_name(argparse_error_code code) {
    switch (code) {
        case ARGPARSE_OK:                   return "OK";
        case ARGPARSE_ERROR_UNKNOWN:        return "UNKNOWN_ERROR";
        case ARGPARSE_ERROR_MISSING_ARGUMENT: return "MISSING_ARGUMENT";
        case ARGPARSE_ERROR_UNEXPECTED_ARGUMENT: return "UNEXPECTED_ARGUMENT";
        case ARGPARSE_ERROR_INVALID_VALUE:  return "INVALID_VALUE";
        case ARGPARSE_ERROR_TOO_MANY_ARGUMENTS: return "TOO_MANY_ARGUMENTS";
        case ARGPARSE_ERROR_AMBIGUOUS_OPTION: return "AMBIGUOUS_OPTION";
        case ARGPARSE_ERROR_CONFLICT:       return "CONFLICT";
        case ARGPARSE_ERROR_OUT_OF_MEMORY:  return "OUT_OF_MEMORY";
        case ARGPARSE_ERROR_INVALID_CONFIG: return "INVALID_CONFIG";
    }
    return "UNRECOGNIZED";
}

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("error_handling", "Error handling demo");

    /* Required option */
    struct argparse_option *name = argparse_add_option(parser, 'n', "name",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Your name (required)", "name");
    argparse_option_set_required(name, true);

    /* Int option for type error demo */
    argparse_add_option(parser, 'p', "port",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Port number", "port");

    /* Enum option for choice error demo */
    struct argparse_option *level = argparse_add_option(parser, 'l', "level",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_ENUM, "Log level", "level");
    const char *levels[] = {"debug", "info", "warn", "error"};
    argparse_option_set_choices(level, levels, 4);

    struct argparse_result *result = argparse_parse(parser, argc, argv);
    argparse_error_code errcode = argparse_result_error_code(result);

    if (errcode != ARGPARSE_OK) {
        fprintf(stderr, "Parse failed!\n");
        fprintf(stderr, "  Error code: %s (%d)\n", error_code_name(errcode), errcode);
        fprintf(stderr, "  Message:    %s\n", argparse_result_error(result));
        fprintf(stderr, "\nHint: Run with --help for usage information.\n");
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    printf("Success!\n");
    printf("  name:  %s\n", argparse_get_string(result, "name"));
    printf("  port:  %d\n", argparse_get_int(result, "port"));
    printf("  level: %s\n", argparse_get_enum(result, "level"));

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
