/*
 * examples/large_cli.c - Realistic large CLI
 *
 * Demonstrates a realistic CLI application with many options,
 * subcommands, groups, and various features combined.
 *
 * Run:
 *   ./large_cli build --target release --jobs 8
 *   ./large_cli test --filter "unit" --verbose
 *   ./large_cli clean --all
 *   ./large_cli --help
 */

#include <stdio.h>
#include <argparse-c/argparse.h>

static void on_build(struct argparse *parser, void *data) {
    (void)parser; (void)data;
    printf("Building...\n");
}

static void on_test(struct argparse *parser, void *data) {
    (void)parser; (void)data;
    printf("Running tests...\n");
}

static void on_clean(struct argparse *parser, void *data) {
    (void)parser; (void)data;
    printf("Cleaning...\n");
}

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("mybuild", "A build system CLI");
    argparse_set_epilog(parser, "Use 'mybuild <command> --help' for command-specific help.");

    /* Global options */
    struct argparse_group *global = argparse_add_group(parser, "Global Options", NULL);
    struct argparse_option *verbose = argparse_add_option(parser, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose output", "verbose");
    argparse_option_set_action(verbose, ARGPARSE_COUNT);
    argparse_group_add_option(global, verbose);

    struct argparse_option *quiet = argparse_add_option(parser, 'q', "quiet",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Quiet mode", "quiet");
    argparse_option_set_action(quiet, ARGPARSE_STORE_TRUE);
    argparse_group_add_option(global, quiet);

    argparse_add_option(parser, 0, "version",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Show version", NULL);

    /* build subcommand */
    struct argparse_command *cmd_build = argparse_add_command(parser,
        "build", "Build the project", on_build);
    struct argparse *sub_build = argparse_command_get_parser(cmd_build);

    struct argparse_group *build_opts = argparse_add_group(sub_build, "Build Options", NULL);
    struct argparse_option *target = argparse_add_option(sub_build, 't', "target",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_ENUM, "Build target", "target");
    const char *targets[] = {"debug", "release", "relwithdebinfo"};
    argparse_option_set_choices(target, targets, 3);
    argparse_option_set_default(target, "debug");
    argparse_group_add_option(build_opts, target);

    struct argparse_option *jobs = argparse_add_option(sub_build, 'j', "jobs",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Parallel jobs", "jobs");
    argparse_option_set_default(jobs, "4");
    argparse_group_add_option(build_opts, jobs);

    struct argparse_option *clean = argparse_add_option(sub_build, 'c', "clean",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Clean before build", "clean");
    argparse_group_add_option(build_opts, clean);

    /* test subcommand */
    struct argparse_command *cmd_test = argparse_add_command(parser,
        "test", "Run tests", on_test);
    struct argparse *sub_test = argparse_command_get_parser(cmd_test);

    struct argparse_group *test_opts = argparse_add_group(sub_test, "Test Options", NULL);
    argparse_add_option(sub_test, 'f', "filter",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Test name filter", "filter");
    struct argparse_option *test_verbose = argparse_add_option(sub_test, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose test output", "verbose");
    argparse_option_set_action(test_verbose, ARGPARSE_COUNT);
    struct argparse_option *timeout = argparse_add_option(sub_test, 'T', "timeout",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Test timeout (seconds)", "timeout");
    argparse_option_set_default(timeout, "30");
    argparse_group_add_option(test_opts, test_verbose);
    argparse_group_add_option(test_opts, timeout);

    /* clean subcommand */
    struct argparse_command *cmd_clean = argparse_add_command(parser,
        "clean", "Clean build artifacts", on_clean);
    struct argparse *sub_clean = argparse_command_get_parser(cmd_clean);

    struct argparse_option *all = argparse_add_option(sub_clean, 'a', "all",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Remove everything", "all");
    argparse_option_set_action(all, ARGPARSE_STORE_TRUE);

    struct argparse_option *build_dir = argparse_add_option(sub_clean, 'd', "build-dir",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Build directory", "build_dir");
    argparse_option_set_default(build_dir, "build");

    struct argparse_result *result = argparse_parse(parser, argc, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    printf("\nParsed successfully.\n");
    printf("  verbose: %d\n", argparse_get_int(result, "verbose"));
    printf("  quiet:   %s\n", argparse_get_bool(result, "quiet") ? "true" : "false");

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
