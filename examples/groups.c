/*
 * examples/groups.c - Argument groups and mutex groups
 *
 * Demonstrates organizing options into logical groups for help output,
 * and using mutually exclusive options.
 *
 * Run:
 *   ./groups --input in.txt --output out.txt
 *   ./groups --verbose --output out.txt
 *   ./groups --quiet --output out.txt
 *   ./groups --verbose --quiet  # Error: mutually exclusive
 */

#include <stdio.h>
#include <argparse-c/argparse.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("groups", "Argument groups demo");

    /* Create display groups */
    struct argparse_group *input_group = argparse_add_group(parser,
        "Input Options", "Options controlling input");
    struct argparse_group *output_group = argparse_add_group(parser,
        "Output Options", "Options controlling output");
    struct argparse_group *verbosity_group = argparse_add_group(parser,
        "Verbosity", "Control output verbosity");

    /* Input group options */
    struct argparse_option *input = argparse_add_option(parser, 'i', "input",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Input file", "input");
    argparse_group_add_option(input_group, input);

    struct argparse_option *recursive = argparse_add_option(parser, 'r', "recursive",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Recurse into directories", "recursive");
    argparse_group_add_option(input_group, recursive);

    /* Output group options */
    struct argparse_option *output = argparse_add_option(parser, 'o', "output",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output file", "output");
    argparse_group_add_option(output_group, output);

    struct argparse_option *format = argparse_add_option(parser, 'f', "format",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output format", "format");
    argparse_group_add_option(output_group, format);

    /* Verbosity group with mutex */
    struct argparse_mutex *verb_mutex = argparse_add_mutex(parser, verbosity_group);
    struct argparse_option *verbose = argparse_add_option(parser, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose output", "verbose");
    struct argparse_option *quiet = argparse_add_option(parser, 'q', "quiet",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Quiet output", "quiet");
    argparse_mutex_add_option(verb_mutex, verbose);
    argparse_mutex_add_option(verb_mutex, quiet);

    struct argparse_result *result = argparse_parse(parser, argc, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    printf("input:     %s\n", argparse_get_string(result, "input"));
    printf("output:    %s\n", argparse_get_string(result, "output"));
    printf("recursive: %s\n", argparse_get_bool(result, "recursive") ? "yes" : "no");
    printf("format:    %s\n", argparse_get_string(result, "format"));
    printf("verbose:   %s\n", argparse_get_bool(result, "verbose") ? "yes" : "no");
    printf("quiet:     %s\n", argparse_get_bool(result, "quiet") ? "yes" : "no");

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
