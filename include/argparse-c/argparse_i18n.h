/**
 * @file argparse_i18n.h
 * @brief Localization (i18n) support for the argparse-c library.
 *
 * Provides functions to set the locale and register translations for
 * standard help text messages such as "usage", "options", and error
 * messages.
 */

#ifndef ARGPARSE_I18N_H
#define ARGPARSE_I18N_H

#include "argparse_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Set the locale for help text.
 *
 * Sets the active locale used for translating standard messages in
 * help output and error messages. The default locale is "en" (English).
 *
 * @param[in] locale The locale identifier (e.g., "en", "es", "fr").
 *                   Must not be NULL. If the locale has no registered
 *                   translations, English strings are used as fallback.
 *
 * @thread_safety Not thread-safe. The locale is global state.
 */
void argparse_set_locale(const char *locale);

/**
 * @brief Get the current locale.
 *
 * @return Pointer to the current locale string. Never NULL.
 *
 * @note The returned string is owned by the library and must not be freed.
 * @thread_safety Safe to read concurrently.
 */
const char *argparse_get_locale(void);

/**
 * @brief Get a translated string for a known message key.
 *
 * Looks up the translation for the given key in the current locale.
 * If no translation exists, falls back to the English default, and
 * if that also does not exist, returns the key itself.
 *
 * @param[in] key The message key (e.g., ARGPARSE_MSG_USAGE). Must not
 *                be NULL.
 *
 * @return Pointer to the translated string. Never NULL.
 *
 * @note The returned string is owned by the library and must not be freed.
 * @thread_safety Safe to read concurrently.
 */
const char *argparse_i18n_get(const char *key);

/**
 * @brief Register a translation for a key in a specific locale.
 *
 * Adds or replaces a translation for the given key in the specified
 * locale.
 *
 * @param[in] locale      The locale identifier (e.g., "es"). Must not be NULL.
 * @param[in] key         The message key (e.g., ARGPARSE_MSG_USAGE). Must not
 *                        be NULL.
 * @param[in] translation The translated string. Must not be NULL.
 *
 * @return 0 on success, -1 on failure (registry full or allocation error).
 *
 * @thread_safety Not thread-safe. The translation table is global state.
 */
int argparse_i18n_set(const char *locale, const char *key,
                       const char *translation);

/** @brief Message key for "help" text. */
#define ARGPARSE_MSG_HELP "help"

/** @brief Message key for "usage" text. */
#define ARGPARSE_MSG_USAGE "usage"

/** @brief Message key for "options" section title. */
#define ARGPARSE_MSG_OPTIONS "options"

/** @brief Message key for "positional arguments" section title. */
#define ARGPARSE_MSG_POSITIONAL "positional arguments"

/** @brief Message key for "commands" section title. */
#define ARGPARSE_MSG_COMMANDS "commands"

/** @brief Message key for "version" text. */
#define ARGPARSE_MSG_VERSION "version"

/** @brief Message key for "error" prefix. */
#define ARGPARSE_MSG_ERROR "error"

/** @brief Message key for "required" text. */
#define ARGPARSE_MSG_REQUIRED "required"

/** @brief Message key for "invalid" text. */
#define ARGPARSE_MSG_INVALID "invalid"

/** @brief Message key for "missing" text. */
#define ARGPARSE_MSG_MISSING "missing"

/** @brief Message key for "unexpected" text. */
#define ARGPARSE_MSG_UNEXPECTED "unexpected"

/** @brief Message key for "conflicting options" text. */
#define ARGPARSE_MSG_CONFLICT "conflicting options"

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_I18N_H */
