#include "test_framework.h"
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void test_color_enable_disable(void) {
    argparse_color_enable(true);
    ASSERT_TRUE(argparse_color_enabled());
    argparse_color_enable(false);
    ASSERT_FALSE(argparse_color_enabled());
    argparse_color_enable(true);
    ASSERT_TRUE(argparse_color_enabled());
}

static void test_color_code_red(void) {
    argparse_color_enable(true);
    const char *code = argparse_color_code(ARGPARSE_COLOR_RED);
    ASSERT_NOT_NULL(code);
    ASSERT_STR_CONTAINS(code, "\033[");
}

static void test_color_code_default(void) {
    argparse_color_enable(true);
    const char *code = argparse_color_code(ARGPARSE_COLOR_DEFAULT);
    ASSERT_NOT_NULL(code);
    ASSERT_STR_EQ(code, "");
}

static void test_color_code_disabled(void) {
    argparse_color_enable(false);
    const char *code = argparse_color_code(ARGPARSE_COLOR_RED);
    ASSERT_NOT_NULL(code);
    ASSERT_STR_EQ(code, "");
    argparse_color_enable(true);
}

static void test_print_colored(void) {
    argparse_color_enable(true);
    argparse_print_colored(stdout, "test", ARGPARSE_COLOR_GREEN);
    ASSERT_TRUE(1);
}

static void test_all_colors(void) {
    argparse_color_enable(true);
    argparse_color code;
    for (code = ARGPARSE_COLOR_DEFAULT; code <= ARGPARSE_COLOR_BOLD; code++) {
        const char *c = argparse_color_code(code);
        ASSERT_NOT_NULL(c);
    }
}

int main(void) {
    printf("=== test_color ===\n");
    RUN_TEST(test_color_enable_disable);
    RUN_TEST(test_color_code_red);
    RUN_TEST(test_color_code_default);
    RUN_TEST(test_color_code_disabled);
    RUN_TEST(test_print_colored);
    RUN_TEST(test_all_colors);
    printf("Results: %d/%d passed\n", _tests_passed, _tests_run);
    return _tests_failed > 0 ? 1 : 0;
}
