/**
 * @file argparse_format.h
 * @brief Help formatting and output.
 *
 * Provides functions to print formatted help, usage, and version information
 * to stdout or a custom output stream.
 */

#ifndef ARGPARSE_FORMAT_H
#define ARGPARSE_FORMAT_H

#include "argparse_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Print formatted help information to stdout.
 *
 * Outputs the full help message including program name, usage line,
 * description, option groups with their options, and epilog. The output
 * is formatted to fit within 80 columns.
 *
 * @param[in] parser The parser whose help to print. Must not be NULL.
 *
 * @note Output is written to stdout (or the configured output stream).
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_print_help(const struct argparse *parser);

/**
 * @brief Print the usage summary to stdout.
 *
 * Outputs a single-line usage summary showing the program name and its
 * expected arguments. This is shorter than the full help output.
 *
 * @param[in] parser The parser whose usage to print. Must not be NULL.
 *
 * @note Output is written to stdout (or the configured output stream).
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_print_usage(const struct argparse *parser);

/**
 * @brief Print version information to stdout.
 *
 * Outputs the version string configured for the parser. Typically used
 * with the --version option (ARGPARSE_VERSION action).
 *
 * @param[in] parser The parser whose version to print. Must not be NULL.
 *
 * @note Output is written to stdout (or the configured output stream).
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_print_version(const struct argparse *parser);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_FORMAT_H */
