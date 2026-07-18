#include "test_framework.h"
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <string.h>

static bool parse_email(const char *value, void *output,
                        char *error_buf, size_t error_buf_size) {
    const char *at = strchr(value, '@');
    if (!at || strchr(at + 1, '.') == NULL) {
        snprintf(error_buf, error_buf_size, "invalid email: %s", value);
        return false;
    }
    *(const char **)output = value;
    return true;
}

static void test_register_custom_type(void) {
    int rc = argparse_register_type("email", parse_email);
    ASSERT_EQ(rc, 0);
}

static void test_unregister_custom_type(void) {
    argparse_register_type("temp_type", parse_email);
    argparse_unregister_type("temp_type");
}

int main(void) {
    printf("=== test_plugin ===\n");
    RUN_TEST(test_register_custom_type);
    RUN_TEST(test_unregister_custom_type);
    printf("Results: %d/%d passed\n", _tests_passed, _tests_run);
    return _tests_failed > 0 ? 1 : 0;
}
