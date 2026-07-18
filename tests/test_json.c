#include "test_framework.h"
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <string.h>

static void test_export_schema_stdout(void) {
    struct argparse *parser = argparse_new("test", "test program");
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose output", NULL);
    argparse_add_option(parser, 'n', "count", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Count", "count");
    int rc = argparse_export_json_schema(parser, stdout);
    ASSERT_EQ(rc, 0);
    argparse_free(parser);
}

static void test_export_schema_file(void) {
    struct argparse *parser = argparse_new("test", "test program");
    argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Output file", "output");
    int rc = argparse_export_json_schema_file(parser, "test_schema.json");
    ASSERT_EQ(rc, 0);
    argparse_free(parser);
    remove("test_schema.json");
}

static void test_export_values(void) {
    struct argparse *parser = argparse_new("test", "test");
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose", NULL);
    argparse_add_option(parser, 'n', "count", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Count", "count");

    const char *argv[] = {"test", "-v", "-n", "42"};
    struct argparse_result *result = argparse_parse(parser, 4, argv);
    ASSERT_NOT_NULL(result);
    int rc = argparse_export_json_values(result, stdout);
    ASSERT_EQ(rc, 0);
    argparse_result_free(result);
    argparse_free(parser);
}

static void test_export_values_file(void) {
    struct argparse *parser = argparse_new("test", "test");
    argparse_add_option(parser, 'n', "count", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Count", "count");

    const char *argv[] = {"test", "-n", "99"};
    struct argparse_result *result = argparse_parse(parser, 3, argv);
    ASSERT_NOT_NULL(result);
    int rc = argparse_export_json_values_file(result, "test_values.json");
    ASSERT_EQ(rc, 0);
    argparse_result_free(result);
    argparse_free(parser);
    remove("test_values.json");
}

int main(void) {
    printf("=== test_json ===\n");
    RUN_TEST(test_export_schema_stdout);
    RUN_TEST(test_export_schema_file);
    RUN_TEST(test_export_values);
    RUN_TEST(test_export_values_file);
    printf("Results: %d/%d passed\n", _tests_passed, _tests_run);
    return _tests_failed > 0 ? 1 : 0;
}
