#include "test_framework.h"
#include <argparse-c/argparse.h>

static void test_store_action(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'n', "name",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Name", "name");
    argparse_option_set_action(opt, ARGPARSE_STORE);
    const char *argv[] = {"prog", "-n", "test"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "name"), "test");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_store_true(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    argparse_option_set_action(opt, ARGPARSE_STORE_TRUE);
    const char *argv[] = {"prog", "-v"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_TRUE(argparse_get_bool(r, "verbose"));
    argparse_result_free(r);
    argparse_free(p);
}

static void test_store_true_not_present(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    argparse_option_set_action(opt, ARGPARSE_STORE_TRUE);
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_FALSE(argparse_get_bool(r, "verbose"));
    argparse_result_free(r);
    argparse_free(p);
}

static void test_store_false(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'q', "quiet",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Quiet", "quiet");
    argparse_option_set_action(opt, ARGPARSE_STORE_FALSE);
    const char *argv[] = {"prog", "-q"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_FALSE(argparse_get_bool(r, "quiet"));
    argparse_result_free(r);
    argparse_free(p);
}

static void test_store_const(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    argparse_option_set_action(opt, ARGPARSE_STORE_CONST);
    argparse_option_set_const(opt, "yes");
    const char *argv[] = {"prog", "-v"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "verbose"), "yes");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_count_single(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbosity", "verbose");
    argparse_option_set_action(opt, ARGPARSE_COUNT);
    const char *argv[] = {"prog", "-v"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_int(r, "verbose"), 1);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_count_multiple(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbosity", "verbose");
    argparse_option_set_action(opt, ARGPARSE_COUNT);
    const char *argv[] = {"prog", "-v", "-v", "-v"};
    struct argparse_result *r = argparse_parse(p, 4, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_int(r, "verbose"), 3);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_count_combined_flags(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbosity", "verbose");
    argparse_option_set_action(opt, ARGPARSE_COUNT);
    const char *argv[] = {"prog", "-vvv"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_int(r, "verbose"), 3);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_count_not_present(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbosity", "verbose");
    argparse_option_set_action(opt, ARGPARSE_COUNT);
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_int(r, "verbose"), 0);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_append_single(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'I', "include",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Include paths", "includes");
    argparse_option_set_action(opt, ARGPARSE_APPEND);
    const char *argv[] = {"prog", "-I", "/usr/include"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_count(r, "includes"), (size_t)1);
    ASSERT_STR_EQ(argparse_get_string_at(r, "includes", 0), "/usr/include");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_append_multiple(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'I', "include",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Include paths", "includes");
    argparse_option_set_action(opt, ARGPARSE_APPEND);
    const char *argv[] = {"prog", "-I", "/usr/include", "-I", "/usr/local/include"};
    struct argparse_result *r = argparse_parse(p, 5, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_count(r, "includes"), (size_t)2);
    ASSERT_STR_EQ(argparse_get_string_at(r, "includes", 0), "/usr/include");
    ASSERT_STR_EQ(argparse_get_string_at(r, "includes", 1), "/usr/local/include");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_help_action(void) {
    struct argparse *p = argparse_new("prog", "Test program");
    argparse_add_option(p, 'h', "help", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Show help", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'H', "Help",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Show help", NULL);
    argparse_option_set_action(opt, ARGPARSE_HELP);
    const char *argv[] = {"prog", "-h"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_NOT_NULL(r);
    argparse_result_free(r);
    argparse_free(p);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    printf("\n=== test_actions ===\n");
    _tests_run = 0; _tests_passed = 0; _tests_failed = 0;

    RUN_TEST(test_store_action);
    RUN_TEST(test_store_true);
    RUN_TEST(test_store_true_not_present);
    RUN_TEST(test_store_false);
    RUN_TEST(test_store_const);
    RUN_TEST(test_count_single);
    RUN_TEST(test_count_multiple);
    RUN_TEST(test_count_combined_flags);
    RUN_TEST(test_count_not_present);
    RUN_TEST(test_append_single);
    RUN_TEST(test_append_multiple);
    RUN_TEST(test_help_action);

    printf("\nResults: %d/%d passed", _tests_passed, _tests_run);
    if (_tests_failed > 0) printf(", %d FAILED", _tests_failed);
    printf("\n");
    return _tests_failed > 0 ? 1 : 0;
}
