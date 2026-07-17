#include "argparse-c/argparse_i18n.h"
#include "argparse_internal.h"

#include <string.h>

#define argparse_MAX_TRANSLATIONS 128

typedef struct argparse_translation_entry {
    char *locale;
    char *key;
    char *translation;
    bool in_use;
} argparse_translation_entry;

static argparse_translation_entry translations[argparse_MAX_TRANSLATIONS];
static char current_locale[32] = "en";
static bool i18n_initialized = false;

static void ensure_initialized(void)
{
    if (!i18n_initialized) {
        memset(translations, 0, sizeof(translations));
        i18n_initialized = true;
    }
}

void argparse_set_locale(const char *locale)
{
    if (!locale) return;
    size_t len = strlen(locale);
    if (len >= sizeof(current_locale)) len = sizeof(current_locale) - 1;
    memcpy(current_locale, locale, len);
    current_locale[len] = '\0';
}

const char *argparse_get_locale(void)
{
    return current_locale;
}

const char *argparse_i18n_get(const char *key)
{
    if (!key) return "";

    ensure_initialized();

    for (int i = 0; i < argparse_MAX_TRANSLATIONS; i++) {
        if (translations[i].in_use &&
            strcmp(translations[i].locale, current_locale) == 0 &&
            strcmp(translations[i].key, key) == 0) {
            return translations[i].translation;
        }
    }

    if (strcmp(current_locale, "en") != 0) {
        for (int i = 0; i < argparse_MAX_TRANSLATIONS; i++) {
            if (translations[i].in_use &&
                strcmp(translations[i].locale, "en") == 0 &&
                strcmp(translations[i].key, key) == 0) {
                return translations[i].translation;
            }
        }
    }

    return key;
}

int argparse_i18n_set(const char *locale, const char *key,
                       const char *translation)
{
    if (!locale || !key || !translation) return -1;

    ensure_initialized();

    for (int i = 0; i < argparse_MAX_TRANSLATIONS; i++) {
        if (translations[i].in_use &&
            strcmp(translations[i].locale, locale) == 0 &&
            strcmp(translations[i].key, key) == 0) {
            argparse_free_internal(translations[i].translation, NULL);
            translations[i].translation = argparse_strdup(translation, NULL);
            if (!translations[i].translation) return -1;
            return 0;
        }
    }

    for (int i = 0; i < argparse_MAX_TRANSLATIONS; i++) {
        if (!translations[i].in_use) {
            translations[i].locale = argparse_strdup(locale, NULL);
            translations[i].key = argparse_strdup(key, NULL);
            translations[i].translation = argparse_strdup(translation, NULL);
            if (!translations[i].locale || !translations[i].key ||
                !translations[i].translation) {
                argparse_free_internal(translations[i].locale, NULL);
                argparse_free_internal(translations[i].key, NULL);
                argparse_free_internal(translations[i].translation, NULL);
                translations[i].locale = NULL;
                translations[i].key = NULL;
                translations[i].translation = NULL;
                return -1;
            }
            translations[i].in_use = true;
            return 0;
        }
    }

    return -1;
}
