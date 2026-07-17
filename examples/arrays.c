/*
 * examples/arrays.c - nargs=+ and nargs=* for multi-value arguments
 *
 * Demonstrates collecting multiple values into arrays using
 * ARGPARSE_NARGS_STAR (zero or more) and ARGPARSE_NARGS_PLUS (one or more).
 *
 * Run:
 *   ./arrays file1.txt file2.txt file3.txt
 *   ./arrays -I /usr/include -I /usr/local/include
 *   ./arrays --include a.h --include b.h
 *   ./arrays                      # nargs=* allows empty
 */

#include <stdio.h>
#include <argparse-c/argparse.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("arrays", "Multi-value arguments demo");

    /* Positional: zero or more files */
    argparse_add_positional(parser,
        ARGPARSE_NARGS_STAR, ARGPARSE_TYPE_STRING,
        "Input files", "files");

    /* -I: append each occurrence */
    struct argparse_option *includes = argparse_add_option(parser, 'I', "include",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Include path", "includes");
    argparse_option_set_action(includes, ARGPARSE_APPEND);

    /* --tag: nargs=? (zero or one) */
    argparse_add_option(parser, 't', "tag",
        ARGPARSE_NARGS_QUESTION, ARGPARSE_TYPE_STRING, "Optional tag", "tag");

    struct argparse_result *result = argparse_parse(parser, argc, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    /* Print collected files */
    size_t file_count = argparse_get_count(result, "files");
    printf("Files (%zu):\n", file_count);
    for (size_t i = 0; i < file_count; i++) {
        printf("  [%zu] %s\n", i, argparse_get_string_at(result, "files", i));
    }

    /* Print collected includes */
    size_t inc_count = argparse_get_count(result, "includes");
    printf("Includes (%zu):\n", inc_count);
    for (size_t i = 0; i < inc_count; i++) {
        printf("  [%zu] %s\n", i, argparse_get_string_at(result, "includes", i));
    }

    /* Print tag */
    const char *tag = argparse_get_string(result, "tag");
    printf("Tag: %s\n", tag ? tag : "(none)");

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
