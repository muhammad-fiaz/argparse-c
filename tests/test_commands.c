#include "test_framework.h"
#include <argparse-c/argparse.h>

static int commit_called = 0;
static int push_called = 0;

static void commit_cb(struct argparse *parser, void *data) {
    (void)parser; (void)data;
    commit_called = 1;
}

static void push_cb(struct argparse *parser, void *data) {
    (void)parser; (void)data;
    push_called = 1;
}

static void test_add_command(void) {
    struct argparse *p = argparse_new("git", "Git version control");
    struct argparse_command *cmd = argparse_add_command(p, "commit", "Record changes", NULL);
    ASSERT_NOT_NULL(cmd);
    argparse_free(p);
}

static void test_command_get_parser(void) {
    struct argparse *p = argparse_new("git", "Git version control");
    struct argparse_command *cmd = argparse_add_command(p, "commit", "Record changes", NULL);
    ASSERT_NOT_NULL(cmd);
    struct argparse *sub = argparse_command_get_parser(cmd);
    ASSERT_NOT_NULL(sub);
    argparse_free(p);
}

static void test_subcommand_basic(void) {
    struct argparse *p = argparse_new("git", "Git version control");
    struct argparse_command *cmd = argparse_add_command(p, "commit", "Record changes", commit_cb);
    struct argparse *sub = argparse_command_get_parser(cmd);
    argparse_add_option(sub, 'm', "message", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Commit message", "message");

    const char *argv[] = {"git", "commit", "-m", "initial commit"};
    struct argparse_result *r = argparse_parse(p, 4, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "message"), "initial commit");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_subcommand_callback(void) {
    commit_called = 0;
    struct argparse *p = argparse_new("git", "Git version control");
    struct argparse_command *cmd = argparse_add_command(p, "commit", "Record changes", commit_cb);
    struct argparse *sub = argparse_command_get_parser(cmd);
    argparse_add_option(sub, 'm', "message", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Message", "message");

    const char *argv[] = {"git", "commit", "-m", "test"};
    struct argparse_result *r = argparse_parse(p, 4, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(commit_called, 1);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_multiple_subcommands(void) {
    commit_called = 0;
    push_called = 0;
    struct argparse *p = argparse_new("git", "Git version control");

    struct argparse_command *c1 = argparse_add_command(p, "commit", "Record changes", commit_cb);
    struct argparse_command *c2 = argparse_add_command(p, "push", "Update remote", push_cb);

    struct argparse *s1 = argparse_command_get_parser(c1);
    argparse_add_option(s1, 'm', "message", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Message", "message");

    struct argparse *s2 = argparse_command_get_parser(c2);
    argparse_add_option(s2, 'f', "force", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Force push", "force");

    const char *argv[] = {"git", "push", "-f"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(push_called, 1);
    ASSERT_EQ(commit_called, 0);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_subcommand_positional(void) {
    struct argparse *p = argparse_new("tool", NULL);
    struct argparse_command *cmd = argparse_add_command(p, "clone", "Clone a repo", NULL);
    struct argparse *sub = argparse_command_get_parser(cmd);
    argparse_add_positional(sub, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Repository URL", "repo");

    const char *argv[] = {"tool", "clone", "https://github.com/user/repo"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "repo"), "https://github.com/user/repo");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_unknown_subcommand(void) {
    struct argparse *p = argparse_new("git", "Git version control");
    argparse_add_command(p, "commit", "Record changes", NULL);

    const char *argv[] = {"git", "unknown"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_NE(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_no_subcommand(void) {
    struct argparse *p = argparse_new("git", "Git version control");
    argparse_add_command(p, "commit", "Record changes", commit_cb);

    const char *argv[] = {"git"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_NOT_NULL(r);
    argparse_result_free(r);
    argparse_free(p);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    printf("\n=== test_commands ===\n");
    _tests_run = 0; _tests_passed = 0; _tests_failed = 0;

    RUN_TEST(test_add_command);
    RUN_TEST(test_command_get_parser);
    RUN_TEST(test_subcommand_basic);
    RUN_TEST(test_subcommand_callback);
    RUN_TEST(test_multiple_subcommands);
    RUN_TEST(test_subcommand_positional);
    RUN_TEST(test_unknown_subcommand);
    RUN_TEST(test_no_subcommand);

    printf("\nResults: %d/%d passed", _tests_passed, _tests_run);
    if (_tests_failed > 0) printf(", %d FAILED", _tests_failed);
    printf("\n");
    return _tests_failed > 0 ? 1 : 0;
}
