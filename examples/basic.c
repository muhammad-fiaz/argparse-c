/*
 * examples/basic.c - Simplest argument parser
 *
 * Demonstrates creating a minimal parser with a single option
 * and a positional argument.
 *
 * Build:
 *   gcc -o basic basic.c -I../include -L../build -largparse-c
 *
 * Run:
 *   ./basic -o output.txt input.txt
 */

#include <stdio.h>
#include <argparse-c/argparse.h>

int main(int argc, const char **argv) {
    /* Create a parser with program name and description */
    struct argparse *parser = argparse_new(
        "basic",
        "A minimal argument parser example"
    );

    /* Add a short option: -o / --output */
    argparse_add_option(parser, 'o', "output",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Output file path", "output");

    /* Add a positional argument */
    argparse_add_positional(parser,
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Input file", "input");

    /* Parse command-line arguments */
    struct argparse_result *result = argparse_parse(parser, argc, argv);

    /* Check for errors */
    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    /* Access parsed values */
    const char *input = argparse_get_string(result, "input");
    const char *output = argparse_get_string(result, "output");

    printf("Input:  %s\n", input ? input : "(none)");
    printf("Output: %s\n", output ? output : "(default)");

    /* Cleanup */
    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
