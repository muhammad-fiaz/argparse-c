#include "test_framework.h"
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <string.h>

static void test_freeze_unfreeze(void) {
    struct argparse *parser = argparse_new("test", "test");
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose", NULL);
    ASSERT_FALSE(argparse_is_frozen(parser));
    argparse_freeze(parser);
    ASSERT_TRUE(argparse_is_frozen(parser));
    argparse_free(parser);
}

static void test_threadsafe_parse(void) {
    struct argparse *parser = argparse_new("test", "test");
    struct argparse_option *verbose_opt = argparse_add_option(
        parser, 'v', "verbose", ARGPARSE_NARGS_0,
        ARGPARSE_TYPE_NONE, "Verbose", NULL);
    argparse_option_set_action(verbose_opt, ARGPARSE_STORE_TRUE);
    argparse_add_option(parser, 'n', "count", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Count", "count");
    argparse_freeze(parser);

    const char *argv[] = {"test", "-v", "-n", "42"};
    struct argparse_result *result = argparse_parse_threadsafe(parser, 4, argv);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(argparse_result_error_code(result), ARGPARSE_OK);
    ASSERT_TRUE(argparse_get_bool(result, "verbose"));
    ASSERT_EQ(argparse_get_int(result, "count"), 42);
    argparse_result_free(result);
    argparse_free(parser);
}

static void test_threadsafe_no_args(void) {
    struct argparse *parser = argparse_new("test", "test");
    struct argparse_option *verbose_opt = argparse_add_option(
        parser, 'v', "verbose", ARGPARSE_NARGS_0,
        ARGPARSE_TYPE_NONE, "Verbose", NULL);
    argparse_option_set_action(verbose_opt, ARGPARSE_STORE_TRUE);
    argparse_freeze(parser);

    const char *argv[] = {"test"};
    struct argparse_result *result = argparse_parse_threadsafe(parser, 1, argv);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(argparse_result_error_code(result), ARGPARSE_OK);
    ASSERT_FALSE(argparse_get_bool(result, "verbose"));
    argparse_result_free(result);
    argparse_free(parser);
}

int main(void) {
    printf("=== test_thread ===\n");
    RUN_TEST(test_freeze_unfreeze);
    RUN_TEST(test_threadsafe_parse);
    RUN_TEST(test_threadsafe_no_args);
    printf("Results: %d/%d passed\n", _tests_passed, _tests_run);
    return _tests_failed > 0 ? 1 : 0;
}
