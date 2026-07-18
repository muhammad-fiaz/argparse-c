---
title: "Commands API"
description: "Subcommand support for multi-command CLI applications. struct argparse_command *argparse_add_command(struct argparse *parser, Add a subcommand with its own parser. struct argparse *argparse_command_get_parser(struct argparse_command *command); Get the subcommand's parser for adding its own options. void init_callback(struct argparse *parser, void *data) { void build_callback(struct argparse *parser, void *data) { int main(int argc, const char **argv) { Usage: mytool init --force mytool build --target release"
---

# Commands API

Subcommand support for multi-command CLI applications.

## Functions

### `argparse_add_command`

```c
struct argparse_command *argparse_add_command(struct argparse *parser,
                                              const char *name,
                                              const char *help,
                                              argparse_callback_fn callback);
```

Add a subcommand with its own parser.

**Parameters:**
- `name` — Subcommand name as it appears on the command line
- `help` — Help text for this subcommand
- `callback` — Function called when this subcommand is selected

**Returns:** Command pointer, or NULL on failure.

### `argparse_command_get_parser`

```c
struct argparse *argparse_command_get_parser(struct argparse_command *command);
```

Get the subcommand's parser for adding its own options.

## Example

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

void init_callback(struct argparse *parser, void *data) {
    struct argparse_result *result = (struct argparse_result *)data;
    printf("Initializing project...\n");
    bool force = argparse_get_bool(result, "force");
    if (force) printf("Forced initialization\n");
}

void build_callback(struct argparse *parser, void *data) {
    struct argparse_result *result = (struct argparse_result *)data;
    const char *target = argparse_get_string(result, "target");
    printf("Building target: %s\n", target ? target : "default");
}

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("mytool", "Build tool");

    /* Add init subcommand */
    struct argparse_command *init_cmd = argparse_add_command(
        parser, "init", "Initialize project", init_callback);
    struct argparse *init_parser = argparse_command_get_parser(init_cmd);
    argparse_add_option(init_parser, 'f', "force", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Force init", "force");

    /* Add build subcommand */
    struct argparse_command *build_cmd = argparse_add_command(
        parser, "build", "Build project", build_callback);
    struct argparse *build_parser = argparse_command_get_parser(build_cmd);
    argparse_add_option(build_parser, 't', "target", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Build target", "target");

    /* Parse and execute */
    struct argparse_result *result = argparse_parse(parser, argc, argv);
    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
    }

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

Usage:

```bash
mytool init --force
mytool build --target release
```

## Notes

- Each subcommand has its own isolated parser
- Options are not shared between subcommands and the parent
- The subcommand's callback receives the parent parse result
- Subcommands can have their own subcommands (nested)
