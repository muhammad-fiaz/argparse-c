#include "test_framework.h"
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <string.h>

static void test_help_does_not_crash(void) {
    struct argparse *p = argparse_new("myprog", "A test program");
    argparse_add_option(p, 'v', "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose output", "verbose");
    argparse_add_option(p, 'o', "output", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output file", "output");
    argparse_print_help(p);
    argparse_free(p);
}

static void test_usage_does_not_crash(void) {
    struct argparse *p = argparse_new("myprog", "A test program");
    argparse_add_option(p, 'v', "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose output", "verbose");
    argparse_print_usage(p);
    argparse_free(p);
}

static void test_help_with_description(void) {
    struct argparse *p = argparse_new("myprog", "This is a test program description");
    argparse_add_option(p, 'v', "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Enable verbose output", "verbose");
    argparse_add_option(p, 'o', "output", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output file path", "output");
    argparse_print_help(p);
    argparse_free(p);
}

static void test_help_with_epilog(void) {
    struct argparse *p = argparse_new("myprog", "Test program");
    argparse_set_epilog(p, "Examples:\n  myprog -o file.txt\n  myprog --verbose");
    argparse_add_option(p, 'o', "output", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output file", "output");
    argparse_print_help(p);
    argparse_free(p);
}

static void test_help_with_groups(void) {
    struct argparse *p = argparse_new("myprog", "Test program");
    struct argparse_group *g1 = argparse_add_group(p, "Input Options", "Options for input");
    struct argparse_group *g2 = argparse_add_group(p, "Output Options", "Options for output");
    struct argparse_option *o1 = argparse_add_option(p, 'i', "input",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Input file", "input");
    struct argparse_option *o2 = argparse_add_option(p, 'o', "output",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output file", "output");
    argparse_group_add_option(g1, o1);
    argparse_group_add_option(g2, o2);
    argparse_print_help(p);
    argparse_free(p);
}

static void test_help_with_positional(void) {
    struct argparse *p = argparse_new("myprog", "Test program");
    argparse_add_positional(p, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Input file", "input");
    argparse_add_option(p, 'v', "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    argparse_print_help(p);
    argparse_free(p);
}

static void test_help_with_metavar(void) {
    struct argparse *p = argparse_new("myprog", "Test program");
    struct argparse_option *opt = argparse_add_option(p, 'o', "output",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output file", "output");
    argparse_option_set_metavar(opt, "FILE");
    argparse_print_help(p);
    argparse_free(p);
}

static void test_help_with_choices(void) {
    struct argparse *p = argparse_new("myprog", "Test program");
    struct argparse_option *opt = argparse_add_option(p, 'c', "color",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_ENUM, "Color scheme", "color");
    const char *choices[] = {"red", "green", "blue"};
    argparse_option_set_choices(opt, choices, 3);
    argparse_print_help(p);
    argparse_free(p);
}

static void test_usage_with_custom_usage(void) {
    struct argparse *p = argparse_new("myprog", "Test program");
    argparse_set_usage(p, "myprog [OPTIONS] FILE");
    argparse_add_option(p, 'v', "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    argparse_print_usage(p);
    argparse_free(p);
}

static void test_help_minimal(void) {
    struct argparse *p = argparse_new("minimal", NULL);
    argparse_print_help(p);
    argparse_free(p);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    printf("\n=== test_format ===\n");
    _tests_run = 0; _tests_passed = 0; _tests_failed = 0;

    RUN_TEST(test_help_does_not_crash);
    RUN_TEST(test_usage_does_not_crash);
    RUN_TEST(test_help_with_description);
    RUN_TEST(test_help_with_epilog);
    RUN_TEST(test_help_with_groups);
    RUN_TEST(test_help_with_positional);
    RUN_TEST(test_help_with_metavar);
    RUN_TEST(test_help_with_choices);
    RUN_TEST(test_usage_with_custom_usage);
    RUN_TEST(test_help_minimal);

    printf("\nResults: %d/%d passed", _tests_passed, _tests_run);
    if (_tests_failed > 0) printf(", %d FAILED", _tests_failed);
    printf("\n");
    return _tests_failed > 0 ? 1 : 0;
}
