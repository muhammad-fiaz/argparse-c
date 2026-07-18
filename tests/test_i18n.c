#include "test_framework.h"
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <string.h>

static void test_set_locale(void) {
    argparse_set_locale("es");
    ASSERT_STR_EQ(argparse_get_locale(), "es");
    argparse_set_locale("en");
    ASSERT_STR_EQ(argparse_get_locale(), "en");
}

static void test_i18n_set_get(void) {
    int rc = argparse_i18n_set("fr", ARGPARSE_MSG_USAGE, "Utilisation");
    ASSERT_EQ(rc, 0);
    rc = argparse_i18n_set("fr", ARGPARSE_MSG_OPTIONS, "Options");
    ASSERT_EQ(rc, 0);
}

static void test_i18n_translate(void) {
    argparse_i18n_set("de", ARGPARSE_MSG_USAGE, "Benutzung");
    argparse_set_locale("de");
    const char *val = argparse_i18n_get(ARGPARSE_MSG_USAGE);
    ASSERT_NOT_NULL(val);
    ASSERT_STR_EQ(val, "Benutzung");
    argparse_set_locale("en");
}

static void test_i18n_fallback(void) {
    argparse_set_locale("xx_nonexistent");
    const char *val = argparse_i18n_get(ARGPARSE_MSG_USAGE);
    ASSERT_NOT_NULL(val);
    ASSERT_STR_EQ(val, "usage");
}

static void test_i18n_keys_exist(void) {
    const char *keys[] = {
        ARGPARSE_MSG_HELP, ARGPARSE_MSG_USAGE, ARGPARSE_MSG_OPTIONS,
        ARGPARSE_MSG_POSITIONAL, ARGPARSE_MSG_COMMANDS, ARGPARSE_MSG_VERSION,
        ARGPARSE_MSG_ERROR, ARGPARSE_MSG_REQUIRED, ARGPARSE_MSG_INVALID,
        ARGPARSE_MSG_MISSING, ARGPARSE_MSG_UNEXPECTED, ARGPARSE_MSG_CONFLICT
    };
    for (int i = 0; i < 12; i++) {
        const char *val = argparse_i18n_get(keys[i]);
        ASSERT_NOT_NULL(val);
        ASSERT_TRUE(strlen(val) > 0);
    }
}

int main(void) {
    printf("=== test_i18n ===\n");
    RUN_TEST(test_set_locale);
    RUN_TEST(test_i18n_set_get);
    RUN_TEST(test_i18n_translate);
    RUN_TEST(test_i18n_fallback);
    RUN_TEST(test_i18n_keys_exist);
    printf("Results: %d/%d passed\n", _tests_passed, _tests_run);
    return _tests_failed > 0 ? 1 : 0;
}
