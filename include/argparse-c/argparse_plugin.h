/**
 * @file argparse_plugin.h
 * @brief Plugin system for custom type handlers.
 *
 * Provides a registry for custom type handlers that extend the built-in
 * type system. Custom types can be registered globally and used with
 * any parser.
 */

#ifndef ARGPARSE_PLUGIN_H
#define ARGPARSE_PLUGIN_H

#include "argparse_types.h"

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Custom type parse callback function.
 *
 * Called to parse a string value into the custom type. The output buffer
 * is a caller-provided region where the parsed value is stored.
 *
 * @param[in] value          The string value to parse. Must not be NULL.
 * @param[out] output        Pointer to the output buffer where the parsed
 *                           value is stored. Must not be NULL.
 * @param[out] error_buf     Buffer to receive an error message on failure.
 * @param[in] error_buf_size Size of the error buffer in bytes.
 *
 * @return true if the value was parsed successfully, false on error.
 */
typedef bool (*argparse_type_parse_fn)(const char *value, void *output,
                                       char *error_buf, size_t error_buf_size);

/**
 * @brief Base value for custom type identifiers.
 *
 * Custom type values used with argparse_option_set_type should be
 * greater than or equal to this value to avoid conflicts with
 * built-in types.
 */
#define ARGPARSE_TYPE_CUSTOM_BASE 1000

/**
 * @brief Register a custom type handler.
 *
 * Adds a custom type handler to the global registry. The handler is
 * identified by name and can be looked up by parsers that use custom
 * types.
 *
 * @param[in] type_name The name of the custom type. Must not be NULL.
 *                      Copied internally.
 * @param[in] parse_fn  The parse function for this type. Must not be NULL.
 *
 * @return 0 on success, -1 on failure (registry full or allocation error).
 *
 * @note The maximum number of registered custom types is 32.
 * @note Registering a type with a name that already exists replaces the
 *       existing handler.
 * @thread_safety Not thread-safe. The global registry is shared state.
 */
int argparse_register_type(const char *type_name,
                            argparse_type_parse_fn parse_fn);

/**
 * @brief Unregister a custom type handler.
 *
 * Removes a custom type handler from the global registry. If the type
 * is not registered, this is a no-op.
 *
 * @param[in] type_name The name of the type to unregister. Must not be NULL.
 *
 * @thread_safety Not thread-safe. The global registry is shared state.
 */
void argparse_unregister_type(const char *type_name);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_PLUGIN_H */
