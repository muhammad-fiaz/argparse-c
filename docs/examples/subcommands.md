---
title: "Subcommands"
description: "Multi-command CLI applications like git or docker. void init_callback(struct argparse *parser, void *data) { void build_callback(struct argparse *parser, void *data) { void deploy_callback(struct argparse *parser, void *data) { int main(int argc, const char **argv) { Initializing project in ./src... Building project... Deploying..."
---

# Subcommands

Multi-command CLI applications like git or docker.

## Complete Example

```c
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <string.h>

void init_callback(struct argparse *parser, void *data) {
    struct argparse_result *result = (struct argparse_result *)data;
    bool force = argparse_get_bool(result, "force");
    const char *dir = argparse_get_string(result, "dir");

    printf("Initializing project");
    if (dir) printf(" in %s", dir);
    printf("...\n");
    if (force) printf("  (forced)\n");
}

void build_callback(struct argparse *parser, void *data) {
    struct argparse_result *result = (struct argparse_result *)data;
    const char *target = argparse_get_string(result, "target");
    bool release = argparse_get_bool(result, "release");

    printf("Building project...\n");
    printf("  Target: %s\n", target ? target : "default");
    printf("  Mode: %s\n", release ? "release" : "debug");
}

void deploy_callback(struct argparse *parser, void *data) {
    struct argparse_result *result = (struct argparse_result *)data;
    const char *env = argparse_get_string(result, "env");
    const char *version = argparse_get_string(result, "version");

    printf("Deploying...\n");
    printf("  Environment: %s\n", env ? env : "staging");
    printf("  Version: %s\n", version ? version : "latest");
}

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("mytool", "Project management tool");

    /* init subcommand */
    struct argparse_command *init_cmd = argparse_add_command(
        parser, "init", "Initialize a new project", init_callback);
    struct argparse *init_parser = argparse_command_get_parser(init_cmd);
    argparse_add_option(init_parser, 'f', "force", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Force initialization", "force");
    argparse_add_option(init_parser, 'd', "dir", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Target directory", "dir");

    /* build subcommand */
    struct argparse_command *build_cmd = argparse_add_command(
        parser, "build", "Build the project", build_callback);
    struct argparse *build_parser = argparse_command_get_parser(build_cmd);
    argparse_add_option(build_parser, 't', "target", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Build target", "target");
    argparse_add_option(build_parser, 'r', "release", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Release build", "release");

    /* deploy subcommand */
    struct argparse_command *deploy_cmd = argparse_add_command(
        parser, "deploy", "Deploy the project", deploy_callback);
    struct argparse *deploy_parser = argparse_command_get_parser(deploy_cmd);
    argparse_add_option(deploy_parser, 'e', "env", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Target environment", "env");
    argparse_add_option(deploy_parser, 0, "version", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Version to deploy", "version");

    struct argparse_result *result = argparse_parse(parser, argc, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
    }

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Usage

```bash
$ ./mytool init --force --dir ./src
Initializing project in ./src...
  (forced)

$ ./mytool build --target release --release
Building project...
  Target: release
  Mode: release

$ ./mytool deploy --env production --version 1.2.3
Deploying...
  Environment: production
  Version: 1.2.3
```

## Key Points

- `argparse_add_command()` creates a subcommand with its own parser
- `argparse_command_get_parser()` returns the subcommand's parser
- Each subcommand can have its own options and positionals
- The callback receives the parent parse result
- Help is automatically generated for all subcommands
