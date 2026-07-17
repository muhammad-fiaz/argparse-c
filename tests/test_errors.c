#include "test_framework.h"
#include <argparse-c/argparse.h>

static void test_missing_required_option(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'n', "name",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Name", "name");
    argparse_option_set_required(opt, true);
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_NE(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_missing_required_satisfied(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'n', "name",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Name", "name");
    argparse_option_set_required(opt, true);
    const char *argv[] = {"prog", "-n", "test"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_invalid_int_value(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'p', "port", ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Port", "port");
    const char *argv[] = {"prog", "-p", "not_a_number"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_NE(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_invalid_uint_negative(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 's', "size", ARGPARSE_NARGS_1, ARGPARSE_TYPE_UINT, "Size", "size");
    const char *argv[] = {"prog", "-s", "-5"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_NE(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_invalid_float_value(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'r', "rate", ARGPARSE_NARGS_1, ARGPARSE_TYPE_FLOAT, "Rate", "rate");
    const char *argv[] = {"prog", "-r", "xyz"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_NE(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_invalid_enum_choice(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'c', "color",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_ENUM, "Color", "color");
    const char *choices[] = {"red", "green", "blue"};
    argparse_option_set_choices(opt, choices, 3);
    const char *argv[] = {"prog", "-c", "yellow"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_NE(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_error_message_not_null_on_error(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'p', "port", ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Port", "port");
    const char *argv[] = {"prog", "-p", "abc"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_NE(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_NOT_NULL(argparse_result_error(r));
    argparse_result_free(r);
    argparse_free(p);
}

static void test_error_message_null_on_success(void) {
    struct argparse *p = argparse_new("prog", NULL);
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_NULL(argparse_result_error(r));
    argparse_result_free(r);
    argparse_free(p);
}

static void test_error_code_ok(void) {
    struct argparse *p = argparse_new("prog", NULL);
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_error_code_various(void) {
    ASSERT_EQ(ARGPARSE_OK, 0);
    ASSERT_NE(ARGPARSE_ERROR_UNKNOWN, 0);
    ASSERT_NE(ARGPARSE_ERROR_MISSING_ARGUMENT, 0);
    ASSERT_NE(ARGPARSE_ERROR_INVALID_VALUE, 0);
    ASSERT_NE(ARGPARSE_ERROR_CONFLICT, 0);
}

static void test_error_string_all_codes(void) {
    const char *s;
    s = argparse_error_string(ARGPARSE_OK);
    ASSERT_NOT_NULL(s);
    s = argparse_error_string(ARGPARSE_ERROR_UNKNOWN);
    ASSERT_NOT_NULL(s);
    s = argparse_error_string(ARGPARSE_ERROR_MISSING_ARGUMENT);
    ASSERT_NOT_NULL(s);
    s = argparse_error_string(ARGPARSE_ERROR_UNEXPECTED_ARGUMENT);
    ASSERT_NOT_NULL(s);
    s = argparse_error_string(ARGPARSE_ERROR_INVALID_VALUE);
    ASSERT_NOT_NULL(s);
    s = argparse_error_string(ARGPARSE_ERROR_TOO_MANY_ARGUMENTS);
    ASSERT_NOT_NULL(s);
    s = argparse_error_string(ARGPARSE_ERROR_AMBIGUOUS_OPTION);
    ASSERT_NOT_NULL(s);
    s = argparse_error_string(ARGPARSE_ERROR_CONFLICT);
    ASSERT_NOT_NULL(s);
    s = argparse_error_string(ARGPARSE_ERROR_OUT_OF_MEMORY);
    ASSERT_NOT_NULL(s);
    s = argparse_error_string(ARGPARSE_ERROR_INVALID_CONFIG);
    ASSERT_NOT_NULL(s);
}

static void test_mutex_conflict_error(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_mutex *m = argparse_add_mutex(p, NULL);
    struct argparse_option *o1 = argparse_add_option(p, 'a', "alpha",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Alpha", "alpha");
    struct argparse_option *o2 = argparse_add_option(p, 'b', "beta",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Beta", "beta");
    argparse_mutex_add_option(m, o1);
    argparse_mutex_add_option(m, o2);

    const char *argv[] = {"prog", "-a", "-b"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_NE(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    printf("\n=== test_errors ===\n");
    _tests_run = 0; _tests_passed = 0; _tests_failed = 0;

    RUN_TEST(test_missing_required_option);
    RUN_TEST(test_missing_required_satisfied);
    RUN_TEST(test_invalid_int_value);
    RUN_TEST(test_invalid_uint_negative);
    RUN_TEST(test_invalid_float_value);
    RUN_TEST(test_invalid_enum_choice);
    RUN_TEST(test_error_message_not_null_on_error);
    RUN_TEST(test_error_message_null_on_success);
    RUN_TEST(test_error_code_ok);
    RUN_TEST(test_error_code_various);
    RUN_TEST(test_error_string_all_codes);
    RUN_TEST(test_mutex_conflict_error);

    printf("\nResults: %d/%d passed", _tests_passed, _tests_run);
    if (_tests_failed > 0) printf(", %d FAILED", _tests_failed);
    printf("\n");
    return _tests_failed > 0 ? 1 : 0;
}
