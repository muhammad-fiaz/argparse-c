/*
 * examples/advanced.c - Callbacks and advanced features
 *
 * Demonstrates option callbacks, store_const, and combining
 * multiple advanced features.
 *
 * Run:
 *   ./advanced --log
 *   ./advanced --log-level debug
 *   ./advanced --config /etc/myapp.conf
 *   ./advanced --dry-run --verbose
 */

#include <stdio.h>
#include <argparse-c/argparse.h>

static int log_count = 0;

static void on_log(struct argparse *parser, void *data) {
    (void)parser; (void)data;
    log_count++;
    printf("[callback] Log requested (level %d)\n", log_count);
}

static void on_config_loaded(struct argparse *parser, void *data) {
    (void)parser;
    const char *path = (const char *)data;
    printf("[callback] Loading config from: %s\n", path ? path : "(default)");
}

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("advanced", "Advanced features demo");

    /* Option with callback */
    struct argparse_option *log = argparse_add_option(parser, 'l', "log",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Enable logging", "log");
    argparse_option_set_callback(log, on_log, NULL);

    /* store_const action */
    struct argparse_option *dry_run = argparse_add_option(parser, 'n', "dry-run",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Dry run mode", "dry_run");
    argparse_option_set_action(dry_run, ARGPARSE_STORE_CONST);
    argparse_option_set_const(dry_run, "yes");

    /* Verbosity count */
    struct argparse_option *verbose = argparse_add_option(parser, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbosity level", "verbose");
    argparse_option_set_action(verbose, ARGPARSE_COUNT);

    /* Config file with callback */
    struct argparse_option *config = argparse_add_option(parser, 'c', "config",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Config file path", "config");
    argparse_option_set_callback(config, on_config_loaded, "/etc/myapp.conf");
    argparse_option_set_default(config, "/etc/myapp.conf");

    /* Required option */
    struct argparse_option *name = argparse_add_option(parser, 'N', "name",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Application name", "name");
    argparse_option_set_required(name, true);

    /* Metavar example */
    struct argparse_option *output = argparse_add_option(parser, 'o', "output",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output path", "output");
    argparse_option_set_metavar(output, "DIR/FILE");

    struct argparse_result *result = argparse_parse(parser, argc, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    printf("\nParsed values:\n");
    printf("  name:     %s\n", argparse_get_string(result, "name"));
    printf("  verbose:  %d\n", argparse_get_int(result, "verbose"));
    printf("  dry_run:  %s\n", argparse_get_string(result, "dry_run"));
    printf("  config:   %s\n", argparse_get_string(result, "config"));
    printf("  output:   %s\n", argparse_get_string(result, "output"));
    printf("  log callbacks invoked: %d\n", log_count);

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
