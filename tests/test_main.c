#include "test_framework.h"
#include <argparse-c/argparse.h>

/* Test declarations from each test file */
extern void test_version_string(void);
extern void test_version_components(void);

static void test_main_version_string(void) {
    const char *ver = argparse_version_string();
    ASSERT_NOT_NULL(ver);
    ASSERT_STR_EQ(ver, "0.1.0");
}

static void test_main_version_components(void) {
    int major = 0, minor = 0, patch = 0;
    argparse_version(&major, &minor, &patch);
    ASSERT_EQ(major, 0);
    ASSERT_EQ(minor, 1);
    ASSERT_EQ(patch, 0);
}

static void test_main_basic_new_free(void) {
    struct argparse *p = argparse_new("prog", "desc");
    ASSERT_NOT_NULL(p);
    argparse_free(p);
}

static void test_main_parse_empty(void) {
    struct argparse *p = argparse_new("prog", NULL);
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_NOT_NULL(r);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_main_short_option(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'v', "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    const char *argv[] = {"prog", "-v"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_main_long_option(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'o', "output", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output", "output");
    const char *argv[] = {"prog", "--output", "file.txt"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "output"), "file.txt");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_main_positional(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_positional(p, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Input", "input");
    const char *argv[] = {"prog", "data.txt"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "input"), "data.txt");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_main_int_type(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'p', "port", ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Port", "port");
    const char *argv[] = {"prog", "-p", "8080"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_int(r, "port"), 8080);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_main_bool_type(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'f', "flag", ARGPARSE_NARGS_1, ARGPARSE_TYPE_BOOL, "Flag", "flag");
    const char *argv[] = {"prog", "-f", "true"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_TRUE(argparse_get_bool(r, "flag"));
    argparse_result_free(r);
    argparse_free(p);
}

static void test_main_store_true(void) {
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

static void test_main_count(void) {
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

static void test_main_group(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_group *g = argparse_add_group(p, "Options", "Test options");
    struct argparse_option *o = argparse_add_option(p, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    argparse_group_add_option(g, o);
    argparse_free(p);
}

static void test_main_mutex(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_mutex *m = argparse_add_mutex(p, NULL);
    struct argparse_option *o1 = argparse_add_option(p, 'a', "alpha",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Alpha", "alpha");
    struct argparse_option *o2 = argparse_add_option(p, 'b', "beta",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Beta", "beta");
    argparse_mutex_add_option(m, o1);
    argparse_mutex_add_option(m, o2);

    const char *argv[] = {"prog", "-a"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_main_command(void) {
    struct argparse *p = argparse_new("git", "Version control");
    struct argparse_command *cmd = argparse_add_command(p, "commit", "Record changes", NULL);
    ASSERT_NOT_NULL(cmd);
    struct argparse *sub = argparse_command_get_parser(cmd);
    ASSERT_NOT_NULL(sub);
    argparse_free(p);
}

static void test_main_error_handling(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'p', "port", ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Port", "port");
    const char *argv[] = {"prog", "-p", "abc"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_NE(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_NOT_NULL(argparse_result_error(r));
    argparse_result_free(r);
    argparse_free(p);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    printf("\n=== test_main (integration) ===\n");
    _tests_run = 0; _tests_passed = 0; _tests_failed = 0;

    RUN_TEST(test_main_version_string);
    RUN_TEST(test_main_version_components);
    RUN_TEST(test_main_basic_new_free);
    RUN_TEST(test_main_parse_empty);
    RUN_TEST(test_main_short_option);
    RUN_TEST(test_main_long_option);
    RUN_TEST(test_main_positional);
    RUN_TEST(test_main_int_type);
    RUN_TEST(test_main_bool_type);
    RUN_TEST(test_main_store_true);
    RUN_TEST(test_main_count);
    RUN_TEST(test_main_group);
    RUN_TEST(test_main_mutex);
    RUN_TEST(test_main_command);
    RUN_TEST(test_main_error_handling);

    printf("\nResults: %d/%d passed", _tests_passed, _tests_run);
    if (_tests_failed > 0) printf(", %d FAILED", _tests_failed);
    printf("\n");
    return _tests_failed > 0 ? 1 : 0;
}
