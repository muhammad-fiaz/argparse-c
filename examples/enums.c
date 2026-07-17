/*
 * examples/enums.c - Enum choices
 *
 * Demonstrates using ARGPARSE_TYPE_ENUM with argparse_option_set_choices()
 * to restrict option values to a predefined set.
 *
 * Run:
 *   ./enums --color red
 *   ./enums --color blue
 *   ./enums --color yellow  # Error: invalid choice
 *   ./enums                # Uses default "auto"
 */

#include <stdio.h>
#include <argparse-c/argparse.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("enums", "Enum choices demo");

    /* Color enum with choices */
    struct argparse_option *color = argparse_add_option(parser, 'c', "color",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_ENUM, "Color scheme (red, green, blue, auto)", "color");
    const char *colors[] = {"red", "green", "blue", "auto"};
    argparse_option_set_choices(color, colors, 4);
    argparse_option_set_default(color, "auto");

    /* Log level enum */
    struct argparse_option *log = argparse_add_option(parser, 'l', "log-level",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_ENUM, "Log level (debug, info, warn, error)", "log_level");
    const char *levels[] = {"debug", "info", "warn", "error"};
    argparse_option_set_choices(log, levels, 4);
    argparse_option_set_default(log, "info");

    /* Mode enum */
    struct argparse_option *mode = argparse_add_option(parser, 'm', "mode",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_ENUM, "Operating mode (read, write, append)", "mode");
    const char *modes[] = {"read", "write", "append"};
    argparse_option_set_choices(mode, modes, 3);

    struct argparse_result *result = argparse_parse(parser, argc, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    printf("color:     %s\n", argparse_get_enum(result, "color"));
    printf("log-level: %s\n", argparse_get_enum(result, "log_level"));
    printf("mode:      %s\n", argparse_get_enum(result, "mode"));

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
