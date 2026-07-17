#include "test_framework.h"
#include <argparse-c/argparse.h>

static void test_short_option(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'v', NULL, ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    const char *argv[] = {"prog", "-v"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_NOT_NULL(r);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_long_option(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 0, "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    const char *argv[] = {"prog", "--verbose"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_NOT_NULL(r);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_short_option_with_value(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'o', "output", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output file", "output");
    const char *argv[] = {"prog", "-o", "file.txt"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_NOT_NULL(r);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    const char *val = argparse_get_string(r, "output");
    ASSERT_NOT_NULL(val);
    ASSERT_STR_EQ(val, "file.txt");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_long_option_with_value(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'o', "output", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output file", "output");
    const char *argv[] = {"prog", "--output", "file.txt"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_NOT_NULL(r);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "output"), "file.txt");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_long_option_equals_syntax(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'o', "output", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output", "output");
    const char *argv[] = {"prog", "--output=file.txt"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_NOT_NULL(r);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "output"), "file.txt");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_combined_short_flags(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'v', "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    argparse_add_option(p, 'd', "debug", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Debug", "debug");
    argparse_add_option(p, 'f', "force", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Force", "force");
    const char *argv[] = {"prog", "-vdf"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_NOT_NULL(r);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_int_option(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'c', "count", ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Count", "count");
    const char *argv[] = {"prog", "-c", "42"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_int(r, "count"), 42);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_int_option_long(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'c', "count", ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Count", "count");
    const char *argv[] = {"prog", "--count", "100"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_int(r, "count"), 100);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_negative_int_option(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'o', "offset", ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Offset", "offset");
    const char *argv[] = {"prog", "-o", "-5"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_int(r, "offset"), -5);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_multiple_options(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'n', "name", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Name", "name");
    argparse_add_option(p, 'a', "age", ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Age", "age");
    const char *argv[] = {"prog", "-n", "Alice", "-a", "30"};
    struct argparse_result *r = argparse_parse(p, 5, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "name"), "Alice");
    ASSERT_EQ(argparse_get_int(r, "age"), 30);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_option_with_default(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'o', "output",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output", "output");
    argparse_option_set_default(opt, "default.txt");
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "output"), "default.txt");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_option_override_default(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'o', "output",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output", "output");
    argparse_option_set_default(opt, "default.txt");
    const char *argv[] = {"prog", "-o", "custom.txt"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "output"), "custom.txt");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_short_only_option(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'x', NULL, ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Extra", "x");
    const char *argv[] = {"prog", "-x"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_long_only_option(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 0, "extra", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Extra", "extra");
    const char *argv[] = {"prog", "--extra", "val"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "extra"), "val");
    argparse_result_free(r);
    argparse_free(p);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    printf("\n=== test_options ===\n");
    _tests_run = 0; _tests_passed = 0; _tests_failed = 0;

    RUN_TEST(test_short_option);
    RUN_TEST(test_long_option);
    RUN_TEST(test_short_option_with_value);
    RUN_TEST(test_long_option_with_value);
    RUN_TEST(test_long_option_equals_syntax);
    RUN_TEST(test_combined_short_flags);
    RUN_TEST(test_int_option);
    RUN_TEST(test_int_option_long);
    RUN_TEST(test_negative_int_option);
    RUN_TEST(test_multiple_options);
    RUN_TEST(test_option_with_default);
    RUN_TEST(test_option_override_default);
    RUN_TEST(test_short_only_option);
    RUN_TEST(test_long_only_option);

    printf("\nResults: %d/%d passed", _tests_passed, _tests_run);
    if (_tests_failed > 0) printf(", %d FAILED", _tests_failed);
    printf("\n");
    return _tests_failed > 0 ? 1 : 0;
}
