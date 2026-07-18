#include "test_framework.h"
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <string.h>

static void test_option_set_range_int(void) {
    struct argparse *parser = argparse_new("test", "test");
    struct argparse_option *opt = argparse_add_option(
        parser, 'n', "count", ARGPARSE_NARGS_1,
        ARGPARSE_TYPE_INT, "Count", "count");
    argparse_option_set_range_int(opt, 1, 100);

    const char *argv[] = {"test", "-n", "50"};
    struct argparse_result *result = argparse_parse(parser, 3, argv);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(argparse_result_error_code(result), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_int(result, "count"), 50);
    argparse_result_free(result);

    const char *argv2[] = {"test", "-n", "200"};
    struct argparse_result *result2 = argparse_parse(parser, 3, argv2);
    ASSERT_NOT_NULL(result2);
    ASSERT_NE(argparse_result_error_code(result2), ARGPARSE_OK);
    argparse_result_free(result2);
    argparse_free(parser);
}

static void test_option_set_range_float(void) {
    struct argparse *parser = argparse_new("test", "test");
    struct argparse_option *opt = argparse_add_option(
        parser, 'r', "rate", ARGPARSE_NARGS_1,
        ARGPARSE_TYPE_FLOAT, "Rate", "rate");
    argparse_option_set_range_float(opt, 0.0f, 1.0f);

    const char *argv[] = {"test", "-r", "0.5"};
    struct argparse_result *result = argparse_parse(parser, 3, argv);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(argparse_result_error_code(result), ARGPARSE_OK);
    argparse_result_free(result);
    argparse_free(parser);
}

static void test_option_set_pattern(void) {
    struct argparse *parser = argparse_new("test", "test");
    struct argparse_option *opt = argparse_add_option(
        parser, 'f', "file", ARGPARSE_NARGS_1,
        ARGPARSE_TYPE_STRING, "File", "file");
    argparse_option_set_pattern(opt, "*.txt");

    const char *argv[] = {"test", "-f", "hello.txt"};
    struct argparse_result *result = argparse_parse(parser, 3, argv);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(argparse_result_error_code(result), ARGPARSE_OK);
    argparse_result_free(result);
    argparse_free(parser);
}

static void test_validate_nonempty(void) {
    char err[256];
    ASSERT_TRUE(argparse_validate_nonempty("hello", err, sizeof(err)));
    ASSERT_FALSE(argparse_validate_nonempty("", err, sizeof(err)));
}

int main(void) {
    printf("=== test_validators_extra ===\n");
    RUN_TEST(test_option_set_range_int);
    RUN_TEST(test_option_set_range_float);
    RUN_TEST(test_option_set_pattern);
    RUN_TEST(test_validate_nonempty);
    printf("Results: %d/%d passed\n", _tests_passed, _tests_run);
    return _tests_failed > 0 ? 1 : 0;
}
