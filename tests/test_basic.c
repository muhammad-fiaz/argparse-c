#include "test_framework.h"
#include <argparse-c/argparse.h>
#include <string.h>

static void test_version_string(void) {
    const char *ver = argparse_version_string();
    ASSERT_NOT_NULL(ver);
    ASSERT_STR_EQ(ver, "0.1.0");
}

static void test_version_components(void) {
    int major = 0, minor = 0, patch = 0;
    argparse_version(&major, &minor, &patch);
    ASSERT_EQ(major, 0);
    ASSERT_EQ(minor, 1);
    ASSERT_EQ(patch, 0);
}

static void test_new_and_free(void) {
    struct argparse *p = argparse_new("myprog", "A test program");
    ASSERT_NOT_NULL(p);
    argparse_free(p);
}

static void test_new_null_prog(void) {
    struct argparse *p = argparse_new(NULL, NULL);
    ASSERT_NOT_NULL(p);
    argparse_free(p);
}

static void test_set_prog(void) {
    struct argparse *p = argparse_new("old", NULL);
    ASSERT_NOT_NULL(p);
    argparse_set_prog(p, "new");
    argparse_free(p);
}

static void test_set_description(void) {
    struct argparse *p = argparse_new("prog", "old desc");
    ASSERT_NOT_NULL(p);
    argparse_set_description(p, "new desc");
    argparse_free(p);
}

static void test_set_epilog(void) {
    struct argparse *p = argparse_new("prog", NULL);
    ASSERT_NOT_NULL(p);
    argparse_set_epilog(p, "some epilog");
    argparse_free(p);
}

static void test_set_usage(void) {
    struct argparse *p = argparse_new("prog", NULL);
    ASSERT_NOT_NULL(p);
    argparse_set_usage(p, "usage: prog [OPTIONS]");
    argparse_free(p);
}

static void test_set_prefix_chars(void) {
    struct argparse *p = argparse_new("prog", NULL);
    ASSERT_NOT_NULL(p);
    argparse_set_prefix_chars(p, "-/");
    argparse_free(p);
}

static void test_set_fromfile_prefix_chars(void) {
    struct argparse *p = argparse_new("prog", NULL);
    ASSERT_NOT_NULL(p);
    argparse_set_fromfile_prefix_chars(p, "@");
    argparse_free(p);
}

static void test_add_option_returns_non_null(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *opt = argparse_add_option(p, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose output", "verbose");
    ASSERT_NOT_NULL(opt);
    argparse_free(p);
}

static void test_add_positional_returns_non_null(void) {
    struct argparse *p = argparse_new("prog", NULL);
    struct argparse_option *pos = argparse_add_positional(p,
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Input file", "input");
    ASSERT_NOT_NULL(pos);
    argparse_free(p);
}

static void test_parse_no_args(void) {
    struct argparse *p = argparse_new("prog", NULL);
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_NOT_NULL(r);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_parse_free_null(void) {
    argparse_result_free(NULL);
}

static void test_free_null(void) {
    argparse_free(NULL);
}

static void test_get_string_default_returns_null(void) {
    struct argparse *p = argparse_new("prog", NULL);
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_NULL(argparse_get_string(r, "nonexistent"));
    argparse_result_free(r);
    argparse_free(p);
}

static void test_get_int_default_returns_zero(void) {
    struct argparse *p = argparse_new("prog", NULL);
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_EQ(argparse_get_int(r, "nonexistent"), 0);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_get_bool_default_returns_false(void) {
    struct argparse *p = argparse_new("prog", NULL);
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_FALSE(argparse_get_bool(r, "nonexistent"));
    argparse_result_free(r);
    argparse_free(p);
}

static void test_get_uint_default_returns_zero(void) {
    struct argparse *p = argparse_new("prog", NULL);
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_EQ(argparse_get_uint(r, "nonexistent"), 0u);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_get_float_default_returns_zero(void) {
    struct argparse *p = argparse_new("prog", NULL);
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_FLOAT_EQ(argparse_get_float(r, "nonexistent"), 0.0f);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_get_double_default_returns_zero(void) {
    struct argparse *p = argparse_new("prog", NULL);
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_DOUBLE_EQ(argparse_get_double(r, "nonexistent"), 0.0);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_get_count_default_returns_zero(void) {
    struct argparse *p = argparse_new("prog", NULL);
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_EQ(argparse_get_count(r, "nonexistent"), (size_t)0);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_error_string(void) {
    const char *s = argparse_error_string(ARGPARSE_OK);
    ASSERT_NOT_NULL(s);
    s = argparse_error_string(ARGPARSE_ERROR_MISSING_ARGUMENT);
    ASSERT_NOT_NULL(s);
    s = argparse_error_string(ARGPARSE_ERROR_INVALID_VALUE);
    ASSERT_NOT_NULL(s);
}

static void test_result_error_on_success(void) {
    struct argparse *p = argparse_new("prog", NULL);
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_NULL(argparse_result_error(r));
    argparse_result_free(r);
    argparse_free(p);
}

static void test_utf8_strlen_ascii(void) {
    ASSERT_EQ(argparse_utf8_strlen("hello"), (size_t)5);
    ASSERT_EQ(argparse_utf8_strlen(""), (size_t)0);
    ASSERT_EQ(argparse_utf8_strlen("a"), (size_t)1);
}

static void test_utf8_strlen_multibyte(void) {
    ASSERT_EQ(argparse_utf8_strlen("\xC3\xA9"), (size_t)1);
    ASSERT_EQ(argparse_utf8_strlen("\xE4\xB8\xAD"), (size_t)1);
}

static void test_complete_bash(void) {
    struct argparse *p = argparse_new("prog", "Test");
    argparse_add_option(p, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose", NULL);
    const char *argv[] = {"prog"};
    argparse_complete_bash(p, 1, argv);
    argparse_free(p);
}

static void test_complete_zsh(void) {
    struct argparse *p = argparse_new("prog", "Test");
    argparse_add_option(p, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose", NULL);
    const char *argv[] = {"prog"};
    argparse_complete_zsh(p, 1, argv);
    argparse_free(p);
}

static void test_complete_fish(void) {
    struct argparse *p = argparse_new("prog", "Test");
    argparse_add_option(p, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose", NULL);
    const char *argv[] = {"prog"};
    argparse_complete_fish(p, 1, argv);
    argparse_free(p);
}

static void test_complete_powershell(void) {
    struct argparse *p = argparse_new("prog", "Test");
    argparse_add_option(p, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose", NULL);
    const char *argv[] = {"prog"};
    argparse_complete_powershell(p, 1, argv);
    argparse_free(p);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    printf("\n=== test_basic ===\n");
    _tests_run = 0; _tests_passed = 0; _tests_failed = 0;

    RUN_TEST(test_version_string);
    RUN_TEST(test_version_components);
    RUN_TEST(test_new_and_free);
    RUN_TEST(test_new_null_prog);
    RUN_TEST(test_set_prog);
    RUN_TEST(test_set_description);
    RUN_TEST(test_set_epilog);
    RUN_TEST(test_set_usage);
    RUN_TEST(test_set_prefix_chars);
    RUN_TEST(test_set_fromfile_prefix_chars);
    RUN_TEST(test_add_option_returns_non_null);
    RUN_TEST(test_add_positional_returns_non_null);
    RUN_TEST(test_parse_no_args);
    RUN_TEST(test_parse_free_null);
    RUN_TEST(test_free_null);
    RUN_TEST(test_get_string_default_returns_null);
    RUN_TEST(test_get_int_default_returns_zero);
    RUN_TEST(test_get_bool_default_returns_false);
    RUN_TEST(test_get_uint_default_returns_zero);
    RUN_TEST(test_get_float_default_returns_zero);
    RUN_TEST(test_get_double_default_returns_zero);
    RUN_TEST(test_get_count_default_returns_zero);
    RUN_TEST(test_error_string);
    RUN_TEST(test_result_error_on_success);
    RUN_TEST(test_utf8_strlen_ascii);
    RUN_TEST(test_utf8_strlen_multibyte);
    RUN_TEST(test_complete_bash);
    RUN_TEST(test_complete_zsh);
    RUN_TEST(test_complete_fish);
    RUN_TEST(test_complete_powershell);

    printf("\nResults: %d/%d passed", _tests_passed, _tests_run);
    if (_tests_failed > 0) printf(", %d FAILED", _tests_failed);
    printf("\n");
    return _tests_failed > 0 ? 1 : 0;
}
