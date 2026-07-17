/**
 * @file argparse_json.h
 * @brief JSON schema and value export.
 *
 * Provides functions to export an argparse parser definition as a JSON
 * schema, and to export parsed values as a JSON object. The JSON output
 * is generated without any external JSON library dependency.
 */

#ifndef ARGPARSE_JSON_H
#define ARGPARSE_JSON_H

#include "argparse_types.h"

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Export the parser schema as JSON to a FILE stream.
 *
 * Writes a JSON object describing the parser's configuration, including
 * the program name, description, and detailed information about each
 * option and positional argument. Each option entry contains:
 * - @c name: Long option name (without prefix).
 * - @c short: Short option character (single character string or null).
 * - @c type: Type name string (e.g., "string", "int", "float").
 * - @c nargs: Number of arguments expected.
 * - @c help: Help text.
 * - @c required: Whether the option is required.
 * - @c default: Default value (if set).
 * - @c choices: Array of allowed choices (if set).
 * - @c dest: Destination name in the result namespace.
 *
 * @param[in] parser The parser to export. Must not be NULL.
 * @param[in] fp     Output FILE stream. Must not be NULL and must be
 *                   open for writing.
 *
 * @return 0 on success, -1 on failure (write error or NULL arguments).
 *
 * @note The output is valid JSON suitable for consumption by JSON parsers.
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
int argparse_export_json_schema(const struct argparse *parser, FILE *fp);

/**
 * @brief Export the parser schema as JSON to a file.
 *
 * Convenience function that opens the specified file, writes the JSON
 * schema, and closes the file.
 *
 * @param[in] parser   The parser to export. Must not be NULL.
 * @param[in] filename Output file path. Must not be NULL. The file will
 *                     be created or overwritten.
 *
 * @return 0 on success, -1 on failure.
 *
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
int argparse_export_json_schema_file(const struct argparse *parser,
                                     const char *filename);

/**
 * @brief Export parsed values as JSON to a FILE stream.
 *
 * Writes a JSON object containing all parsed values from the given
 * result. Each key-value pair corresponds to a destination name and
 * its parsed value. Array values are represented as JSON arrays.
 *
 * @param[in] result The parse result to export. Must not be NULL.
 * @param[in] fp     Output FILE stream. Must not be NULL and must be
 *                   open for writing.
 *
 * @return 0 on success, -1 on failure (write error or NULL arguments).
 *
 * @note The output is valid JSON suitable for consumption by JSON parsers.
 * @thread_safety Safe to read concurrently, provided the result is not
 *                being modified.
 */
int argparse_export_json_values(const struct argparse_result *result,
                                FILE *fp);

/**
 * @brief Export parsed values as JSON to a file.
 *
 * Convenience function that opens the specified file, writes the JSON
 * values, and closes the file.
 *
 * @param[in] result   The parse result to export. Must not be NULL.
 * @param[in] filename Output file path. Must not be NULL. The file will
 *                     be created or overwritten.
 *
 * @return 0 on success, -1 on failure.
 *
 * @thread_safety Safe to read concurrently, provided the result is not
 *                being modified.
 */
int argparse_export_json_values_file(const struct argparse_result *result,
                                     const char *filename);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_JSON_H */
