#include "test_framework.h"
#include <argparse-c/argparse.h>

static void test_add_group(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_group *g = argparse_add_group(p, "Output Options", "Control output format");
    ASSERT_NOT_NULL(g);
    argparse_free(p);
}

static void test_group_with_options(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_group *g = argparse_add_group(p, "Output", "Output options");
    struct argparse_option *opt = argparse_add_option(p, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    argparse_group_add_option(g, opt);
    const char *argv[] = {"prog", "-v"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_multiple_groups(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_group *g1 = argparse_add_group(p, "Input", "Input options");
    struct argparse_group *g2 = argparse_add_group(p, "Output", "Output options");
    ASSERT_NOT_NULL(g1);
    ASSERT_NOT_NULL(g2);

    struct argparse_option *o1 = argparse_add_option(p, 'i', "input",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Input file", "input");
    struct argparse_option *o2 = argparse_add_option(p, 'o', "output",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output file", "output");
    argparse_group_add_option(g1, o1);
    argparse_group_add_option(g2, o2);

    const char *argv[] = {"prog", "-i", "in.txt", "-o", "out.txt"};
    struct argparse_result *r = argparse_parse(p, 5, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "input"), "in.txt");
    ASSERT_STR_EQ(argparse_get_string(r, "output"), "out.txt");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_mutex_group(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_mutex *m = argparse_add_mutex(p, NULL);
    ASSERT_NOT_NULL(m);

    struct argparse_option *o1 = argparse_add_option(p, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    struct argparse_option *o2 = argparse_add_option(p, 'q', "quiet",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Quiet", "quiet");
    argparse_mutex_add_option(m, o1);
    argparse_mutex_add_option(m, o2);

    const char *argv[] = {"prog", "-v"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_mutex_conflict(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_mutex *m = argparse_add_mutex(p, NULL);

    struct argparse_option *o1 = argparse_add_option(p, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    struct argparse_option *o2 = argparse_add_option(p, 'q', "quiet",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Quiet", "quiet");
    argparse_mutex_add_option(m, o1);
    argparse_mutex_add_option(m, o2);

    const char *argv[] = {"prog", "-v", "-q"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_NE(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_mutex_with_group(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_group *g = argparse_add_group(p, "Verbosity", "Verbosity control");
    struct argparse_mutex *m = argparse_add_mutex(p, g);

    struct argparse_option *o1 = argparse_add_option(p, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    struct argparse_option *o2 = argparse_add_option(p, 'q', "quiet",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Quiet", "quiet");
    argparse_mutex_add_option(m, o1);
    argparse_mutex_add_option(m, o2);

    const char *argv[] = {"prog", "-q"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_mutex_neither_present(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_mutex *m = argparse_add_mutex(p, NULL);

    struct argparse_option *o1 = argparse_add_option(p, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    struct argparse_option *o2 = argparse_add_option(p, 'q', "quiet",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Quiet", "quiet");
    argparse_mutex_add_option(m, o1);
    argparse_mutex_add_option(m, o2);

    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    printf("\n=== test_groups ===\n");
    _tests_run = 0; _tests_passed = 0; _tests_failed = 0;

    RUN_TEST(test_add_group);
    RUN_TEST(test_group_with_options);
    RUN_TEST(test_multiple_groups);
    RUN_TEST(test_mutex_group);
    RUN_TEST(test_mutex_conflict);
    RUN_TEST(test_mutex_with_group);
    RUN_TEST(test_mutex_neither_present);

    printf("\nResults: %d/%d passed", _tests_passed, _tests_run);
    if (_tests_failed > 0) printf(", %d FAILED", _tests_failed);
    printf("\n");
    return _tests_failed > 0 ? 1 : 0;
}
