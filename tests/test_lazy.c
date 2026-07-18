#include "test_framework.h"
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <string.h>

static const char *lazy_from_user(void *user_data) {
    (void)user_data;
    return "lazy_value";
}

static const char *lazy_from_env(void *user_data) {
    const char *env = (const char *)user_data;
    const char *val = getenv(env);
    return val ? val : "fallback";
}

static void test_lazy_default_applied(void) {
    struct argparse *parser = argparse_new("test", "test");
    struct argparse_option *opt = argparse_add_option(
        parser, 'o', "output", ARGPARSE_NARGS_1,
        ARGPARSE_TYPE_STRING, "Output", "output");
    argparse_option_set_lazy_default(opt, lazy_from_user, NULL);

    const char *argv[] = {"test"};
    struct argparse_result *result = argparse_parse(parser, 1, argv);
    ASSERT_NOT_NULL(result);
    const char *val = argparse_get_string(result, "output");
    ASSERT_NOT_NULL(val);
    ASSERT_STR_EQ(val, "lazy_value");
    argparse_result_free(result);
    argparse_free(parser);
}

static void test_lazy_not_called_when_set(void) {
    struct argparse *parser = argparse_new("test", "test");
    struct argparse_option *opt = argparse_add_option(
        parser, 'o', "output", ARGPARSE_NARGS_1,
        ARGPARSE_TYPE_STRING, "Output", "output");
    argparse_option_set_lazy_default(opt, lazy_from_user, NULL);

    const char *argv[] = {"test", "-o", "explicit"};
    struct argparse_result *result = argparse_parse(parser, 3, argv);
    ASSERT_NOT_NULL(result);
    const char *val = argparse_get_string(result, "output");
    ASSERT_NOT_NULL(val);
    ASSERT_STR_EQ(val, "explicit");
    argparse_result_free(result);
    argparse_free(parser);
}

static void test_lazy_clear(void) {
    struct argparse *parser = argparse_new("test", "test");
    struct argparse_option *opt = argparse_add_option(
        parser, 'o', "output", ARGPARSE_NARGS_1,
        ARGPARSE_TYPE_STRING, "Output", "output");
    argparse_option_set_lazy_default(opt, lazy_from_user, NULL);
    argparse_option_set_lazy_default(opt, NULL, NULL);

    const char *argv[] = {"test"};
    struct argparse_result *result = argparse_parse(parser, 1, argv);
    ASSERT_NOT_NULL(result);
    const char *val = argparse_get_string(result, "output");
    ASSERT_NULL(val);
    argparse_result_free(result);
    argparse_free(parser);
}

int main(void) {
    printf("=== test_lazy ===\n");
    RUN_TEST(test_lazy_default_applied);
    RUN_TEST(test_lazy_not_called_when_set);
    RUN_TEST(test_lazy_clear);
    printf("Results: %d/%d passed\n", _tests_passed, _tests_run);
    return _tests_failed > 0 ? 1 : 0;
}
