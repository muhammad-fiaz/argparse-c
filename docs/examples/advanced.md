# Advanced Features

Demonstrates callbacks, store_const, required options, and combining multiple advanced features.

## Running

```bash
./advanced --log
./advanced --log-level debug
./advanced --config /etc/myapp.conf
./advanced --dry-run --verbose
```

## What It Demonstrates

- Option callbacks with `argparse_option_set_callback()`
- Store const with `ARGPARSE_STORE_CONST` action
- Required options with `argparse_option_set_required()`
- Combining multiple features in a realistic CLI

## Full Code

```c
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <string.h>

static int dry_run = 0;
static int log_enabled = 0;

static void on_log(struct argparse *parser, void *data) {
    (void)parser; (void)data;
    log_enabled = 1;
    printf("[callback] Logging enabled\n");
}

static void on_dry_run(struct argparse *parser, void *data) {
    (void)parser; (void)data;
    dry_run = 1;
    printf("[callback] Dry run mode\n");
}

int main(void) {
    struct argparse *parser = argparse_new("advanced",
        "Demonstrate advanced features");

    struct argparse_option *log_opt = argparse_add_option(
        parser, 'l', "log", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE,
        "Enable logging", NULL);
    argparse_option_set_action(log_opt, ARGPARSE_STORE_TRUE);
    argparse_option_set_callback(log_opt, on_log, NULL);

    struct argparse_option *log_level_opt = argparse_add_option(
        parser, 0, "log-level", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Log level", "level");
    argparse_option_set_default(log_level_opt, "info");
    const char *levels[] = {"debug", "info", "warn", "error"};
    argparse_option_set_choices(log_level_opt, levels, 4);

    struct argparse_option *config_opt = argparse_add_option(
        0, "config", "config", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Config file", "file");
    argparse_option_set_required(config_opt, true);

    struct argparse_option *dry_run_opt = argparse_add_option(
        0, "dry-run", "dry-run", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE,
        "Dry run mode", NULL);
    argparse_option_set_action(dry_run_opt, ARGPARSE_STORE_CONST);
    argparse_option_set_const(dry_run_opt, "true");
    argparse_option_set_callback(dry_run_opt, on_dry_run, NULL);

    struct argparse_option *verbose_opt = argparse_add_option(
        'v', "verbose", "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE,
        "Verbose output", NULL);
    argparse_option_set_action(verbose_opt, ARGPARSE_STORE_TRUE);

    const char *argv[] = {"advanced", "--config", "/etc/myapp.conf",
                          "--dry-run", "--verbose"};
    struct argparse_result *result = argparse_parse(parser, 5, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    printf("config:   %s\n", argparse_get_string(result, "config"));
    printf("verbose:  %s\n",
           argparse_get_bool(result, "verbose") ? "yes" : "no");
    printf("dry_run:  %s\n", dry_run ? "yes" : "no");
    printf("log:      %s\n", log_enabled ? "yes" : "no");

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```
