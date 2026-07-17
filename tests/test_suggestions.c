#include "test_framework.h"
#include <argparse-c/argparse.h>

static void test_misspelled_option_gives_error(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'v', "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    const char *argv[] = {"prog", "--verbse"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_NE(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_misspelled_option_error_message(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'v', "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    const char *argv[] = {"prog", "--verbse"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_NE(argparse_result_error_code(r), ARGPARSE_OK);
    const char *err = argparse_result_error(r);
    ASSERT_NOT_NULL(err);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_unknown_short_option(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'v', "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    const char *argv[] = {"prog", "-x"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_NE(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_correct_option_still_works(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'v', "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    const char *argv[] = {"prog", "--verbose"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    printf("\n=== test_suggestions ===\n");
    _tests_run = 0; _tests_passed = 0; _tests_failed = 0;

    RUN_TEST(test_misspelled_option_gives_error);
    RUN_TEST(test_misspelled_option_error_message);
    RUN_TEST(test_unknown_short_option);
    RUN_TEST(test_correct_option_still_works);

    printf("\nResults: %d/%d passed", _tests_passed, _tests_run);
    if (_tests_failed > 0) printf(", %d FAILED", _tests_failed);
    printf("\n");
    return _tests_failed > 0 ? 1 : 0;
}
