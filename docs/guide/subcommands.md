---
title: Subcommands
description: Creating nested command structures
---

# Subcommands

Subcommands create nested command structures like `git commit` or `npm install`.

## Basic Subcommands

```c
struct argparse_command *commit_cmd = argparse_add_command(
    parser, "commit", "Record changes to the repository"
);

struct argparse_command *push_cmd = argparse_add_command(
    parser, "push", "Update remote references"
);
```

## Adding Options to Commands

```c
struct argparse_command *commit_cmd = argparse_add_command(
    parser, "commit", "Record changes to the repository"
);

argparse_add_option(argparse_command_get_parser(commit_cmd), 'm', "message", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_STRING, "Commit message", "MSG");
argparse_add_option(argparse_command_get_parser(commit_cmd), 'a', "all", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Stage all changes", NULL);
```

## Nested Subcommands

```c
struct argparse_command *remote_cmd = argparse_add_command(
    parser, "remote", "Manage remote repositories"
);

struct argparse_command *remote_add = argparse_add_command(
    remote_cmd, "add", "Add a new remote"
);

struct argparse_command *remote_remove = argparse_add_command(
    remote_cmd, "remove", "Remove a remote"
);
```

## Example: Git-like CLI

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("git", "Version control system");

    /* commit command */
    struct argparse_command *commit = argparse_add_command(
        parser, "commit", "Record changes to the repository"
    );
    argparse_add_option(argparse_command_get_parser(commit), 'm', "message", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Commit message", "MSG");
    argparse_add_option(argparse_command_get_parser(commit), 'a', "all", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Stage all changes", NULL);

    /* push command */
    struct argparse_command *push = argparse_add_command(
        parser, "push", "Update remote references"
    );
    argparse_add_option(argparse_command_get_parser(push), 'u', "set-upstream", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Set upstream for branch", NULL);

    /* pull command */
    struct argparse_command *pull = argparse_add_command(
        parser, "pull", "Fetch and integrate changes"
    );

    /* status command */
    struct argparse_command *status = argparse_add_command(
        parser, "status", "Show working tree status"
    );

    struct argparse_result *result = argparse_parse(
        parser, argc, argv
    );

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    if (result == NULL || argparse_result_error_code(result) != ARGPARSE_ERROR_UNKNOWN) {
        argparse_result_free(result);
        argparse_free(parser);
        return 0;
    }

    const char *cmd = argparse_get_string(result, "__command__");

    if (strcmp(cmd, "commit") == 0) {
        const char *msg = argparse_get_string(result, "message");
        int all = argparse_get_bool(result, "all");
        printf("Committing%s: %s\n", all ? " all" : "", msg);
    } else if (strcmp(cmd, "push") == 0) {
        printf("Pushing to remote\n");
    } else if (strcmp(cmd, "pull") == 0) {
        printf("Pulling from remote\n");
    } else if (strcmp(cmd, "status") == 0) {
        printf("Showing status\n");
    }

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Help Output

```
Usage: git [OPTIONS] COMMAND [ARGS...]

Version control system

Commands:
  commit    Record changes to the repository
  push      Update remote references
  pull      Fetch and integrate changes
  status    Show working tree status

Options:
  -h, --help      Show this help message
  -V, --version   Show version
```

## Next Steps

- [Help Formatting](./help-formatting) - Customizing help output
- [Error Handling](./error-handling) - Handling errors
- [API: Commands](../api/commands) - Complete API reference
