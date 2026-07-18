# Environment & Config

Demonstrates reading option values from environment variables and config files.

## Running

```bash
MYAPP_OUTPUT_DIR=./out MYAPP_VERBOSE=1 ./env_config
./env_config --output-dir /custom/path
MYAPP_COLOR=blue ./env_config
```

## What It Demonstrates

- `argparse_set_env_prefix()` for automatic env var lookup
- `argparse_option_set_env()` for explicit env var mapping
- Priority: CLI args > env vars > defaults
- Config file loading patterns

## Full Code

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(void) {
    struct argparse *parser = argparse_new("env_config",
        "Demonstrate env vars and config files");

    argparse_set_env_prefix(parser, "MYAPP");

    struct argparse_option *output_opt = argparse_add_option(
        parser, 'o', "output-dir", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Output directory", "output_dir");
    argparse_option_set_env(output_opt, "MYAPP_OUTPUT_DIR");
    argparse_option_set_default(output_opt, "/tmp/output");

    struct argparse_option *verbose_opt = argparse_add_option(
        parser, 'v', "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE,
        "Verbose output", NULL);
    argparse_option_set_action(verbose_opt, ARGPARSE_STORE_TRUE);
    argparse_option_set_env(verbose_opt, "MYAPP_VERBOSE");

    struct argparse_option *color_opt = argparse_add_option(
        0, 'c', "color", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Color", "color");
    argparse_option_set_env(color_opt, "MYAPP_COLOR");
    argparse_option_set_default(color_opt, "auto");

    const char *argv[] = {"env_config"};
    struct argparse_result *result = argparse_parse(parser, 1, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    printf("output-dir: %s\n", argparse_get_string(result, "output_dir"));
    printf("verbose:    %s\n",
           argparse_get_bool(result, "verbose") ? "yes" : "no");
    printf("color:      %s\n", argparse_get_string(result, "color"));

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```
