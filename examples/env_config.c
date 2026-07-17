/*
 * examples/env_config.c - Environment variable configuration
 *
 * Demonstrates reading option values from environment variables
 * when not specified on the command line.
 *
 * Run:
 *   MYAPP_OUTPUT_DIR=./out MYAPP_VERBOSE=1 ./env_config
 *   ./env_config --output-dir /custom/path
 *   MYAPP_COLOR=blue ./env_config
 */

#include <stdio.h>
#include <argparse-c/argparse.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("env_config", "Environment variable demo");

    /* Set prefix for automatic env var lookup */
    argparse_set_env_prefix(parser, "MYAPP");

    /* Output directory - checks MYAPP_OUTPUT_DIR */
    struct argparse_option *output_dir = argparse_add_option(parser, 'o', "output-dir",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output directory", "output_dir");
    argparse_option_set_env(output_dir, "MYAPP_OUTPUT_DIR");
    argparse_option_set_default(output_dir, "./default_output");

    /* Verbose flag - checks MYAPP_VERBOSE */
    struct argparse_option *verbose = argparse_add_option(parser, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Enable verbose output", "verbose");
    argparse_option_set_action(verbose, ARGPARSE_STORE_TRUE);
    argparse_option_set_env(verbose, "MYAPP_VERBOSE");

    /* Color scheme - checks MYAPP_COLOR */
    struct argparse_option *color = argparse_add_option(parser, 'c', "color",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Color scheme", "color");
    argparse_option_set_env(color, "MYAPP_COLOR");
    argparse_option_set_default(color, "auto");

    /* Number of threads - checks MYAPP_THREADS */
    struct argparse_option *threads = argparse_add_option(parser, 'j', "threads",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Number of threads", "threads");
    argparse_option_set_env(threads, "MYAPP_THREADS");
    argparse_option_set_default(threads, "4");

    struct argparse_result *result = argparse_parse(parser, argc, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    printf("output_dir: %s\n", argparse_get_string(result, "output_dir"));
    printf("verbose:    %s\n", argparse_get_bool(result, "verbose") ? "true" : "false");
    printf("color:      %s\n", argparse_get_string(result, "color"));
    printf("threads:    %d\n", argparse_get_int(result, "threads"));

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
