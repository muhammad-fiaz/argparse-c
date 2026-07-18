#include "test_framework.h"
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <string.h>

static void test_batch_register(void) {
    struct argparse *parser = argparse_new("test", "test");

    argparse_option_def defs[] = {
        { 'v', "verbose", NULL, "Verbose output", NULL, NULL,
          ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, ARGPARSE_STORE_TRUE, false },
        { 'o', "output", NULL, "Output file", "FILE", NULL,
          ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, ARGPARSE_STORE, false },
        { 'n', "count", NULL, "Count", "NUM", "1",
          ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, ARGPARSE_STORE, false },
    };

    int rc = argparse_add_options_batch(parser, defs, 3);
    ASSERT_EQ(rc, 0);

    const char *argv[] = {"test", "-v", "-o", "file.txt", "-n", "5"};
    struct argparse_result *result = argparse_parse(parser, 6, argv);
    ASSERT_NOT_NULL(result);
    ASSERT_EQ(argparse_result_error_code(result), ARGPARSE_OK);
    ASSERT_TRUE(argparse_get_bool(result, "verbose"));
    ASSERT_STR_EQ(argparse_get_string(result, "output"), "file.txt");
    ASSERT_EQ(argparse_get_int(result, "count"), 5);
    argparse_result_free(result);
    argparse_free(parser);
}

static void test_batch_single(void) {
    struct argparse *parser = argparse_new("test", "test");

    argparse_option_def defs[] = {
        { 'h', "help", NULL, "Show help", NULL, NULL,
          ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, ARGPARSE_HELP, false },
    };

    int rc = argparse_add_options_batch(parser, defs, 1);
    ASSERT_EQ(rc, 0);
    argparse_free(parser);
}

int main(void) {
    printf("=== test_batch ===\n");
    RUN_TEST(test_batch_register);
    RUN_TEST(test_batch_single);
    printf("Results: %d/%d passed\n", _tests_passed, _tests_run);
    return _tests_failed > 0 ? 1 : 0;
}
