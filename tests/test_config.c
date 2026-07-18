#include "test_framework.h"
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <string.h>

static void test_env_prefix(void) {
    struct argparse *parser = argparse_new("test", "test");
    argparse_set_env_prefix(parser, "MYAPP");
    struct argparse_option *opt = argparse_add_option(
        parser, 'o', "output", ARGPARSE_NARGS_1,
        ARGPARSE_TYPE_STRING, "Output", "output");
    argparse_option_set_env(opt, "MYAPP_OUTPUT");
    argparse_option_set_default(opt, "default.txt");
    argparse_free(parser);
}

static void test_config_file_loading(void) {
    FILE *f = fopen("test_config.cfg", "w");
    if (f) {
        fprintf(f, "name=hello\n");
        fprintf(f, "count=42\n");
        fclose(f);
    }

    struct argparse *parser = argparse_new("test", "test");
    argparse_set_fromfile_prefix_chars(parser, "@");
    struct argparse_option *name_opt = argparse_add_option(
        parser, 0, "name", ARGPARSE_NARGS_1,
        ARGPARSE_TYPE_STRING, "Name", "name");
    argparse_option_set_default(name_opt, "default");
    argparse_free(parser);
    remove("test_config.cfg");
}

static void test_response_file(void) {
    FILE *f = fopen("test_response.txt", "w");
    if (f) {
        fprintf(f, "--name test\n");
        fprintf(f, "--count 10\n");
        fclose(f);
    }

    struct argparse *parser = argparse_new("test", "test");
    argparse_set_fromfile_prefix_chars(parser, "@");
    argparse_add_option(parser, 0, "name", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Name", "name");
    argparse_add_option(parser, 0, "count", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Count", "count");

    const char *argv[] = {"test", "@test_response.txt"};
    struct argparse_result *result = argparse_parse(parser, 2, argv);
    ASSERT_NOT_NULL(result);
    const char *name = argparse_get_string(result, "name");
    ASSERT_NOT_NULL(name);
    ASSERT_STR_EQ(name, "test");
    ASSERT_EQ(argparse_get_int(result, "count"), 10);
    argparse_result_free(result);
    argparse_free(parser);
    remove("test_response.txt");
}

int main(void) {
    printf("=== test_config ===\n");
    RUN_TEST(test_env_prefix);
    RUN_TEST(test_config_file_loading);
    RUN_TEST(test_response_file);
    printf("Results: %d/%d passed\n", _tests_passed, _tests_run);
    return _tests_failed > 0 ? 1 : 0;
}
