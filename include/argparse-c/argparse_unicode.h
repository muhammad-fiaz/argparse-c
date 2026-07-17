/**
 * @file argparse_unicode.h
 * @brief Unicode/UTF-8 support utilities.
 *
 * Provides helper functions for working with UTF-8 encoded strings,
 * which are used throughout the library for help text and display.
 */

#ifndef ARGPARSE_UNICODE_H
#define ARGPARSE_UNICODE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the display width of a UTF-8 string in columns.
 *
 * Counts the number of Unicode codepoints that occupy display columns,
 * accounting for wide characters (CJK) and zero-width characters.
 * This is used for aligning help text output.
 *
 * @param[in] str The UTF-8 string to measure. Must not be NULL.
 *
 * @return The display width in columns.
 *
 * @note This function assumes a terminal that uses East Asian width
 *       semantics. Combining characters and zero-width characters do
 *       not contribute to the width.
 * @note If the string contains invalid UTF-8 sequences, each invalid
 *       byte is counted as one column.
 * @thread_safety Safe to call from multiple threads concurrently.
 */
size_t argparse_utf8_strlen(const char *str);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_UNICODE_H */
