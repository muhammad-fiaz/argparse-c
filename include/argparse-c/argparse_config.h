/**
 * @file argparse_config.h
 * @brief Configuration file and environment variable support.
 *
 * Provides functions to load option values from configuration files
 * and environment variables, giving programs multiple sources of
 * configuration with a well-defined precedence order.
 */

#ifndef ARGPARSE_CONFIG_H
#define ARGPARSE_CONFIG_H

#include "argparse_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Set the environment variable prefix for options.
 *
 * When set, the parser checks for environment variables named
 * PREFIX_OPTIONNAME (with dashes converted to underscores and
 * uppercased) for options that don't appear on the command line.
 *
 * For example, with prefix "MYAPP" and option "--output-file",
 * the parser checks MYAPP_OUTPUT_FILE.
 *
 * @param[in,out] parser The parser to configure. Must not be NULL.
 * @param[in] prefix     The environment variable prefix. Copied internally.
 *                       May be NULL to disable environment variable support.
 *
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_set_env_prefix(struct argparse *parser, const char *prefix);

/**
 * @brief Set a specific environment variable for an option.
 *
 * Overrides the automatic environment variable name derived from the prefix.
 * When the option is not on the command line, the parser checks this
 * environment variable.
 *
 * @param[in,out] option The option to configure. Must not be NULL.
 * @param[in] env_var    The environment variable name. Copied internally.
 *                       May be NULL to clear.
 *
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_option_set_env(struct argparse_option *option,
                             const char *env_var);

/**
 * @brief Load option values from a configuration file.
 *
 * Parses a configuration file and applies its values to the parser's
 * options as if they appeared on the command line. The file format
 * supports:
 *
 * - Lines starting with '#' are comments.
 * - Lines of the form `option = value` set option values.
 * - Sections like `[section]` are ignored (for INI compatibility).
 * - Boolean options can be set with `option = true` or `option = false`.
 *
 * @param[in,out] parser   The parser to load configuration into.
 *                         Must not be NULL.
 * @param[in] filename     Path to the configuration file. Must not be NULL.
 *
 * @return ARGPARSE_OK on success, or an error code on failure.
 *
 * @note File values are overridden by command-line arguments (lower precedence).
 * @note Environment variables override file values but are overridden by
 *       command-line arguments.
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
argparse_error_code argparse_load_config(struct argparse *parser,
                                         const char *filename);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_CONFIG_H */
