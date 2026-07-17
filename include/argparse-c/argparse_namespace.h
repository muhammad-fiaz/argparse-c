/**
 * @file argparse_namespace.h
 * @brief Parse result and value access.
 *
 * Provides the parsing entry point and functions to retrieve parsed
 * values from the result namespace by destination name.
 */

#ifndef ARGPARSE_NAMESPACE_H
#define ARGPARSE_NAMESPACE_H

#include "argparse_types.h"

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup parsing Parsing
 * @{
 */

/**
 * @brief Parse command-line arguments.
 *
 * This is the main entry point for parsing. It processes the given argument
 * array according to the parser's configured options, positionals, and
 * constraints.
 *
 * @param[in] parser The parser to use for parsing. Must not be NULL.
 * @param[in] argc   The argument count (typically from main).
 * @param[in] argv   The argument vector (typically from main). Must not be NULL.
 *
 * @return A newly allocated parse result, or NULL on allocation failure.
 *         Check argparse_result_error_code() for the parse outcome.
 *
 * @note The caller is responsible for freeing the result with
 *       argparse_result_free().
 * @note The parser is NOT consumed or freed by this call and can be reused.
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
struct argparse_result *argparse_parse(struct argparse *parser,
                                       int argc,
                                       const char **argv);

/**
 * @brief Free a parse result and all associated resources.
 *
 * @param[in] result The parse result to free. May be NULL (no-op).
 *
 * @thread_safety The result must not be accessed by other threads during
 *                or after this call.
 */
void argparse_result_free(struct argparse_result *result);

/** @} */ /* end of parsing group */

/**
 * @defgroup value_access Value Access
 * @{
 */

/**
 * @brief Get a string value from the parse result.
 *
 * @param[in] result The parse result. Must not be NULL.
 * @param[in] dest   The destination name. Must not be NULL.
 *
 * @return Pointer to the string value, or NULL if not set.
 *
 * @note The returned string is owned by the result and valid for its lifetime.
 * @thread_safety Safe to read concurrently, provided the result is not
 *                being modified.
 */
const char *argparse_get_string(const struct argparse_result *result,
                                const char *dest);

/**
 * @brief Get an integer value from the parse result.
 *
 * @param[in] result The parse result. Must not be NULL.
 * @param[in] dest   The destination name. Must not be NULL.
 *
 * @return The integer value, or 0 if not set.
 *
 * @thread_safety Safe to read concurrently, provided the result is not
 *                being modified.
 */
int argparse_get_int(const struct argparse_result *result, const char *dest);

/**
 * @brief Get an unsigned integer value from the parse result.
 *
 * @param[in] result The parse result. Must not be NULL.
 * @param[in] dest   The destination name. Must not be NULL.
 *
 * @return The unsigned integer value, or 0 if not set.
 *
 * @thread_safety Safe to read concurrently, provided the result is not
 *                being modified.
 */
unsigned int argparse_get_uint(const struct argparse_result *result,
                               const char *dest);

/**
 * @brief Get a float value from the parse result.
 *
 * @param[in] result The parse result. Must not be NULL.
 * @param[in] dest   The destination name. Must not be NULL.
 *
 * @return The float value, or 0.0f if not set.
 *
 * @thread_safety Safe to read concurrently, provided the result is not
 *                being modified.
 */
float argparse_get_float(const struct argparse_result *result,
                         const char *dest);

/**
 * @brief Get a double value from the parse result.
 *
 * @param[in] result The parse result. Must not be NULL.
 * @param[in] dest   The destination name. Must not be NULL.
 *
 * @return The double value, or 0.0 if not set.
 *
 * @thread_safety Safe to read concurrently, provided the result is not
 *                being modified.
 */
double argparse_get_double(const struct argparse_result *result,
                           const char *dest);

/**
 * @brief Get a boolean value from the parse result.
 *
 * @param[in] result The parse result. Must not be NULL.
 * @param[in] dest   The destination name. Must not be NULL.
 *
 * @return The boolean value, or false if not set.
 *
 * @thread_safety Safe to read concurrently, provided the result is not
 *                being modified.
 */
bool argparse_get_bool(const struct argparse_result *result, const char *dest);

/**
 * @brief Get an enum string value from the parse result.
 *
 * @param[in] result The parse result. Must not be NULL.
 * @param[in] dest   The destination name. Must not be NULL.
 *
 * @return Pointer to the selected choice string, or NULL if not set.
 *
 * @note The returned string is owned by the result and valid for its lifetime.
 * @thread_safety Safe to read concurrently, provided the result is not
 *                being modified.
 */
const char *argparse_get_enum(const struct argparse_result *result,
                              const char *dest);

/** @} */ /* end of value_access group */

/**
 * @defgroup array_access Array Access (nargs=* / nargs=+)
 * @{
 */

/**
 * @brief Get the number of values stored for a multi-value destination.
 *
 * Useful for options with nargs=ARGPARSE_NARGS_STAR or nargs=ARGPARSE_NARGS_PLUS.
 *
 * @param[in] result The parse result. Must not be NULL.
 * @param[in] dest   The destination name. Must not be NULL.
 *
 * @return The number of values stored, or 0 if none.
 *
 * @thread_safety Safe to read concurrently, provided the result is not
 *                being modified.
 */
size_t argparse_get_count(const struct argparse_result *result,
                          const char *dest);

/**
 * @brief Get a string value at a specific index for a multi-value destination.
 *
 * @param[in] result The parse result. Must not be NULL.
 * @param[in] dest   The destination name. Must not be NULL.
 * @param[in] index  The zero-based index into the value array.
 *
 * @return Pointer to the string value at the given index, or NULL if the
 *         index is out of bounds or the value is not a string.
 *
 * @note The returned string is owned by the result and valid for its lifetime.
 * @thread_safety Safe to read concurrently, provided the result is not
 *                being modified.
 */
const char *argparse_get_string_at(const struct argparse_result *result,
                                   const char *dest,
                                   size_t index);

/**
 * @brief Get an integer value at a specific index for a multi-value destination.
 *
 * @param[in] result The parse result. Must not be NULL.
 * @param[in] dest   The destination name. Must not be NULL.
 * @param[in] index  The zero-based index into the value array.
 *
 * @return The integer value at the given index, or 0 if the index is out
 *         of bounds.
 *
 * @thread_safety Safe to read concurrently, provided the result is not
 *                being modified.
 */
int argparse_get_int_at(const struct argparse_result *result,
                        const char *dest,
                        size_t index);

/** @} */ /* end of array_access group */

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_NAMESPACE_H */
