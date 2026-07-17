#include "test_framework.h"
#include <argparse-c/argparse.h>
#include <stdlib.h>
#include <string.h>

static void test_string_type(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'n', "name", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Name", "name");
    const char *argv[] = {"prog", "-n", "hello"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "name"), "hello");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_int_type(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'p', "port", ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Port", "port");
    const char *argv[] = {"prog", "-p", "1234"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_int(r, "port"), 1234);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_int_negative(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'o', "offset", ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Offset", "offset");
    const char *argv[] = {"prog", "-o", "-42"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_int(r, "offset"), -42);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_int_invalid(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'p', "port", ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Port", "port");
    const char *argv[] = {"prog", "-p", "abc"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_NE(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_uint_type(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 's', "size", ARGPARSE_NARGS_1, ARGPARSE_TYPE_UINT, "Size", "size");
    const char *argv[] = {"prog", "-s", "100"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_uint(r, "size"), 100u);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_uint_negative_rejected(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 's', "size", ARGPARSE_NARGS_1, ARGPARSE_TYPE_UINT, "Size", "size");
    const char *argv[] = {"prog", "-s", "-1"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_NE(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_float_type(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'r', "rate", ARGPARSE_NARGS_1, ARGPARSE_TYPE_FLOAT, "Rate", "rate");
    const char *argv[] = {"prog", "-r", "3.14"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_FLOAT_EQ(argparse_get_float(r, "rate"), 3.14f);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_float_invalid(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'r', "rate", ARGPARSE_NARGS_1, ARGPARSE_TYPE_FLOAT, "Rate", "rate");
    const char *argv[] = {"prog", "-r", "notanumber"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_NE(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_double_type(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'p', "precision", ARGPARSE_NARGS_1, ARGPARSE_TYPE_DOUBLE, "Precision", "precision");
    const char *argv[] = {"prog", "-p", "2.718281828"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_DOUBLE_EQ(argparse_get_double(r, "precision"), 2.718281828);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_bool_type_true(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'f', "flag", ARGPARSE_NARGS_1, ARGPARSE_TYPE_BOOL, "Flag", "flag");
    const char *argv[] = {"prog", "-f", "true"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_TRUE(argparse_get_bool(r, "flag"));
    argparse_result_free(r);
    argparse_free(p);
}

static void test_bool_type_false(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'f', "flag", ARGPARSE_NARGS_1, ARGPARSE_TYPE_BOOL, "Flag", "flag");
    const char *argv[] = {"prog", "-f", "false"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_FALSE(argparse_get_bool(r, "flag"));
    argparse_result_free(r);
    argparse_free(p);
}

static void test_bool_type_yes_no(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'f', "flag", ARGPARSE_NARGS_1, ARGPARSE_TYPE_BOOL, "Flag", "flag");
    const char *argv[] = {"prog", "-f", "yes"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_TRUE(argparse_get_bool(r, "flag"));
    argparse_result_free(r);
    argparse_free(p);
}

static void test_bool_type_1_0(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'f', "flag", ARGPARSE_NARGS_1, ARGPARSE_TYPE_BOOL, "Flag", "flag");
    const char *argv[] = {"prog", "-f", "1"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_TRUE(argparse_get_bool(r, "flag"));
    argparse_result_free(r);
    argparse_free(p);
}

static void test_enum_type(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'c', "color",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_ENUM, "Color", "color");
    const char *choices[] = {"red", "green", "blue"};
    argparse_option_set_choices(opt, choices, 3);
    const char *argv[] = {"prog", "-c", "red"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_enum(r, "color"), "red");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_enum_invalid_choice(void) {
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

static void test_string_with_special_chars(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'm', "msg", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Message", "msg");
    const char *argv[] = {"prog", "-m", "hello world with spaces"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "msg"), "hello world with spaces");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_int_zero(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'n', "num", ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Number", "num");
    const char *argv[] = {"prog", "-n", "0"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_int(r, "num"), 0);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_double_scientific_notation(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'v', "val", ARGPARSE_NARGS_1, ARGPARSE_TYPE_DOUBLE, "Value", "val");
    const char *argv[] = {"prog", "-v", "1.5e2"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_DOUBLE_EQ(argparse_get_double(r, "val"), 150.0);
    argparse_result_free(r);
    argparse_free(p);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    printf("\n=== test_types ===\n");
    _tests_run = 0; _tests_passed = 0; _tests_failed = 0;

    RUN_TEST(test_string_type);
    RUN_TEST(test_int_type);
    RUN_TEST(test_int_negative);
    RUN_TEST(test_int_invalid);
    RUN_TEST(test_uint_type);
    RUN_TEST(test_uint_negative_rejected);
    RUN_TEST(test_float_type);
    RUN_TEST(test_float_invalid);
    RUN_TEST(test_double_type);
    RUN_TEST(test_bool_type_true);
    RUN_TEST(test_bool_type_false);
    RUN_TEST(test_bool_type_yes_no);
    RUN_TEST(test_bool_type_1_0);
    RUN_TEST(test_enum_type);
    RUN_TEST(test_enum_invalid_choice);
    RUN_TEST(test_string_with_special_chars);
    RUN_TEST(test_int_zero);
    RUN_TEST(test_double_scientific_notation);

    printf("\nResults: %d/%d passed", _tests_passed, _tests_run);
    if (_tests_failed > 0) printf(", %d FAILED", _tests_failed);
    printf("\n");
    return _tests_failed > 0 ? 1 : 0;
}
