/**
 * @file argparse_batch.h
 * @brief Batch option registration for the argparse-c library.
 *
 * Provides a way to register multiple options at once from an array
 * of definitions, reducing repetitive calls to argparse_add_option().
 */

#ifndef ARGPARSE_BATCH_H
#define ARGPARSE_BATCH_H

#include "argparse_types.h"

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Batch option definition for bulk registration.
 *
 * Defines the properties of a single option to be registered via
 * argparse_add_options_batch(). Each field corresponds to a parameter
 * of argparse_add_option() or subsequent configuration calls.
 */
typedef struct argparse_option_def {
    /** @brief Single-character short option name, or 0 for none. */
    char short_name;

    /** @brief Long option name without prefix (e.g., "verbose"), or NULL. */
    const char *long_name;

    /** @brief Destination name in the result namespace. May be NULL. */
    const char *dest;

    /** @brief Help text for this option. May be NULL. */
    const char *help;

    /** @brief Metavar placeholder for help output. May be NULL. */
    const char *metavar;

    /** @brief Default value as a string. May be NULL. */
    const char *default_value;

    /** @brief Number of arguments the option expects. */
    argparse_nargs nargs;

    /** @brief Type of the value to parse. */
    argparse_type type;

    /** @brief Action to perform when the option is encountered. */
    argparse_action action;

    /** @brief Whether the option is required. */
    bool required;
} argparse_option_def;

/**
 * @brief Register multiple options at once.
 *
 * Iterates over an array of option definitions and registers each one
 * with the parser. This is a convenience wrapper around multiple
 * argparse_add_option() calls.
 *
 * @param[in,out] parser The parser to add options to. Must not be NULL.
 * @param[in] defs       Array of option definitions. Must not be NULL.
 * @param[in] count      Number of definitions in the array. Must be > 0.
 *
 * @return 0 on success, -1 on failure (e.g., allocation failure or
 *         invalid parameters). On failure, any options already registered
 *         in this batch remain registered.
 *
 * @note Each definition must have at least one of short_name or long_name
 *       set (non-zero / non-NULL).
 * @note Options added via this function can be further configured using
 *       the standard option configuration functions.
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
int argparse_add_options_batch(struct argparse *parser,
                                const argparse_option_def *defs,
                                size_t count);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_BATCH_H */
