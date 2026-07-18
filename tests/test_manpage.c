#include "test_framework.h"
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <string.h>

static void test_generate_manpage_stdout(void) {
    struct argparse *parser = argparse_new("test", "Test program");
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose output", NULL);
    argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Output file", "output");
    argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                            "Input file", "input");
    int rc = argparse_generate_manpage(parser, stdout);
    ASSERT_EQ(rc, 0);
    argparse_free(parser);
}

static void test_generate_manpage_file(void) {
    struct argparse *parser = argparse_new("myapp", "My application");
    argparse_add_option(parser, 'h', "help", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Show help", NULL);
    int rc = argparse_generate_manpage_file(parser, "test_manpage.1");
    ASSERT_EQ(rc, 0);
    argparse_free(parser);
    remove("test_manpage.1");
}

int main(void) {
    printf("=== test_manpage ===\n");
    RUN_TEST(test_generate_manpage_stdout);
    RUN_TEST(test_generate_manpage_file);
    printf("Results: %d/%d passed\n", _tests_passed, _tests_run);
    return _tests_failed > 0 ? 1 : 0;
}
