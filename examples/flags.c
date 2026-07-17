/*
 * examples/flags.c - Flags and boolean options
 *
 * Demonstrates store_true, store_false, count actions, and combined flags.
 *
 * Build:
 *   gcc -o flags flags.c -I../include -L../build -largparse-c
 *
 * Run:
 *   ./flags -v          # verbose = true
 *   ./flags -vvv        # verbose count = 3
 *   ./flags -q          # quiet = true
 *   ./flags -v -d       # verbose + debug
 */

#include <stdio.h>
#include <argparse-c/argparse.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("flags", "Flag options demo");

    /* store_true: sets dest to true when flag is present */
    struct argparse_option *verbose = argparse_add_option(parser, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Enable verbose output", "verbose");
    argparse_option_set_action(verbose, ARGPARSE_STORE_TRUE);

    /* store_false: sets dest to false when flag is present */
    struct argparse_option *quiet = argparse_add_option(parser, 'q', "quiet",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Suppress output", "quiet");
    argparse_option_set_action(quiet, ARGPARSE_STORE_FALSE);

    /* count: increments counter for each occurrence */
    struct argparse_option *debug = argparse_add_option(parser, 'd', "debug",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Debug level (repeat for more)", "debug");
    argparse_option_set_action(debug, ARGPARSE_COUNT);

    struct argparse_result *result = argparse_parse(parser, argc, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    printf("verbose: %s\n", argparse_get_bool(result, "verbose") ? "true" : "false");
    printf("quiet:   %s\n", argparse_get_bool(result, "quiet") ? "true" : "false");
    printf("debug:   %d\n", argparse_get_int(result, "debug"));

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
