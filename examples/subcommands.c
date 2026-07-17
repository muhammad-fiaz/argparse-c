/*
 * examples/subcommands.c - Git-like CLI with subcommands
 *
 * Demonstrates adding subcommands like "git commit" and "git push",
 * each with their own parser, options, and callbacks.
 *
 * Run:
 *   ./subcommands commit -m "initial commit"
 *   ./subcommands push --force
 *   ./subcommands status
 */

#include <stdio.h>
#include <argparse-c/argparse.h>

static int commit_count = 0;

static void on_commit(struct argparse *parser, void *data) {
    (void)parser; (void)data;
    commit_count++;
}

static void on_push(struct argparse *parser, void *data) {
    (void)parser; (void)data;
    printf("Pushing to remote...\n");
}

static void on_status(struct argparse *parser, void *data) {
    (void)parser; (void)data;
    printf("Repository status: clean\n");
}

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("vcs", "A simple version control system");

    /* Subcommand: commit */
    struct argparse_command *cmd_commit = argparse_add_command(parser,
        "commit", "Record changes to the repository", on_commit);
    struct argparse *sub_commit = argparse_command_get_parser(cmd_commit);
    argparse_add_option(sub_commit, 'm', "message",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Commit message", "message");
    argparse_add_option(sub_commit, 'a', "all",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Stage all changes", "all");

    /* Subcommand: push */
    struct argparse_command *cmd_push = argparse_add_command(parser,
        "push", "Update remote refs", on_push);
    struct argparse *sub_push = argparse_command_get_parser(cmd_push);
    struct argparse_option *force = argparse_add_option(sub_push, 'f', "force",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Force push", "force");
    argparse_option_set_action(force, ARGPARSE_STORE_TRUE);
    argparse_add_option(sub_push, 'u', "set-upstream",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Set upstream branch", "upstream");

    /* Subcommand: status */
    struct argparse_command *cmd_status = argparse_add_command(parser,
        "status", "Show working tree status", on_status);

    struct argparse_result *result = argparse_parse(parser, argc, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    printf("Commits performed: %d\n", commit_count);
    if (argparse_get_string(result, "message")) {
        printf("Last commit message: %s\n", argparse_get_string(result, "message"));
    }

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
