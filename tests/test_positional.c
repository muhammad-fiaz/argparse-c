#include "test_framework.h"
#include <argparse-c/argparse.h>

static void test_single_positional_string(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_positional(p, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Input file", "input");
    const char *argv[] = {"prog", "file.txt"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "input"), "file.txt");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_single_positional_int(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_positional(p, ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Port number", "port");
    const char *argv[] = {"prog", "8080"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_int(r, "port"), 8080);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_two_positionals(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_positional(p, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Source", "source");
    argparse_add_positional(p, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Destination", "dest");
    const char *argv[] = {"prog", "src.txt", "dst.txt"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "source"), "src.txt");
    ASSERT_STR_EQ(argparse_get_string(r, "dest"), "dst.txt");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_positional_with_options(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'v', "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    argparse_add_positional(p, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Input", "input");
    const char *argv[] = {"prog", "-v", "file.txt"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "input"), "file.txt");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_positional_after_options(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'o', "output", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Output", "output");
    argparse_add_positional(p, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Input", "input");
    const char *argv[] = {"prog", "-o", "out.txt", "in.txt"};
    struct argparse_result *r = argparse_parse(p, 4, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "output"), "out.txt");
    ASSERT_STR_EQ(argparse_get_string(r, "input"), "in.txt");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_nargs_star_positional(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_positional(p, ARGPARSE_NARGS_STAR, ARGPARSE_TYPE_STRING, "Files", "files");
    const char *argv[] = {"prog", "a.txt", "b.txt", "c.txt"};
    struct argparse_result *r = argparse_parse(p, 4, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_count(r, "files"), (size_t)3);
    ASSERT_STR_EQ(argparse_get_string_at(r, "files", 0), "a.txt");
    ASSERT_STR_EQ(argparse_get_string_at(r, "files", 1), "b.txt");
    ASSERT_STR_EQ(argparse_get_string_at(r, "files", 2), "c.txt");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_nargs_star_empty(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_positional(p, ARGPARSE_NARGS_STAR, ARGPARSE_TYPE_STRING, "Files", "files");
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_count(r, "files"), (size_t)0);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_nargs_plus_positional(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_positional(p, ARGPARSE_NARGS_PLUS, ARGPARSE_TYPE_STRING, "Files", "files");
    const char *argv[] = {"prog", "a.txt", "b.txt"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_count(r, "files"), (size_t)2);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_nargs_question_positional(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_positional(p, ARGPARSE_NARGS_QUESTION, ARGPARSE_TYPE_STRING, "File", "file");
    const char *argv[] = {"prog", "file.txt"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "file"), "file.txt");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_nargs_question_no_value(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_positional(p, ARGPARSE_NARGS_QUESTION, ARGPARSE_TYPE_STRING, "File", "file");
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_positional_int_at(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_positional(p, ARGPARSE_NARGS_PLUS, ARGPARSE_TYPE_INT, "Numbers", "nums");
    const char *argv[] = {"prog", "10", "20", "30"};
    struct argparse_result *r = argparse_parse(p, 4, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_count(r, "nums"), (size_t)3);
    ASSERT_EQ(argparse_get_int_at(r, "nums", 0), 10);
    ASSERT_EQ(argparse_get_int_at(r, "nums", 1), 20);
    ASSERT_EQ(argparse_get_int_at(r, "nums", 2), 30);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_nargs_two_positional(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_option(p, 'a', "alpha", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Alpha", "alpha");
    argparse_add_option(p, 'b', "beta", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Beta", "beta");
    argparse_add_positional(p, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Source", "source");
    argparse_add_positional(p, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Destination", "dest");
    const char *argv[] = {"prog", "-a", "x", "-b", "y", "src.txt", "dst.txt"};
    struct argparse_result *r = argparse_parse(p, 7, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_STR_EQ(argparse_get_string(r, "source"), "src.txt");
    ASSERT_STR_EQ(argparse_get_string(r, "dest"), "dst.txt");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_nargs_three_positional(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_positional(p, ARGPARSE_NARGS_STAR, ARGPARSE_TYPE_STRING,
        "Files", "files");
    const char *argv[] = {"prog", "a.txt", "b.txt", "c.txt"};
    struct argparse_result *r = argparse_parse(p, 4, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_count(r, "files"), (size_t)3);
    ASSERT_STR_EQ(argparse_get_string_at(r, "files", 0), "a.txt");
    ASSERT_STR_EQ(argparse_get_string_at(r, "files", 1), "b.txt");
    ASSERT_STR_EQ(argparse_get_string_at(r, "files", 2), "c.txt");
    argparse_result_free(r);
    argparse_free(p);
}

static void test_nargs_two_int_positional(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_positional(p, ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT,
        "Start", "start");
    argparse_add_positional(p, ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT,
        "End", "end");
    const char *argv[] = {"prog", "10", "20"};
    struct argparse_result *r = argparse_parse(p, 3, argv);
    ASSERT_EQ(argparse_result_error_code(r), ARGPARSE_OK);
    ASSERT_EQ(argparse_get_int(r, "start"), 10);
    ASSERT_EQ(argparse_get_int(r, "end"), 20);
    argparse_result_free(r);
    argparse_free(p);
}

static void test_nargs_plus_empty_error(void) {
    struct argparse *p = argparse_new("prog", NULL);
    argparse_add_positional(p, ARGPARSE_NARGS_PLUS, ARGPARSE_TYPE_STRING,
        "Files", "files");
    const char *argv[] = {"prog"};
    struct argparse_result *r = argparse_parse(p, 1, argv);
    ASSERT_NE(argparse_result_error_code(r), ARGPARSE_OK);
    argparse_result_free(r);
    argparse_free(p);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    printf("\n=== test_positional ===\n");
    _tests_run = 0; _tests_passed = 0; _tests_failed = 0;

    RUN_TEST(test_single_positional_string);
    RUN_TEST(test_single_positional_int);
    RUN_TEST(test_two_positionals);
    RUN_TEST(test_positional_with_options);
    RUN_TEST(test_positional_after_options);
    RUN_TEST(test_nargs_star_positional);
    RUN_TEST(test_nargs_star_empty);
    RUN_TEST(test_nargs_plus_positional);
    RUN_TEST(test_nargs_question_positional);
    RUN_TEST(test_nargs_question_no_value);
    RUN_TEST(test_positional_int_at);
    RUN_TEST(test_nargs_two_positional);
    RUN_TEST(test_nargs_three_positional);
    RUN_TEST(test_nargs_two_int_positional);
    RUN_TEST(test_nargs_plus_empty_error);

    printf("\nResults: %d/%d passed", _tests_passed, _tests_run);
    if (_tests_failed > 0) printf(", %d FAILED", _tests_failed);
    printf("\n");
    return _tests_failed > 0 ? 1 : 0;
}
