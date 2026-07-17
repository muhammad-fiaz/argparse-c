/**
 * @file argparse_error.h
 * @brief Error handling and error string utilities.
 *
 * Provides functions to convert error codes to human-readable strings
 * and to retrieve error information from parse results.
 */

#ifndef ARGPARSE_ERROR_H
#define ARGPARSE_ERROR_H

#include "argparse_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Convert an error code to a human-readable error string.
 *
 * @param[in] code The error code to convert.
 * @return Pointer to a short, static error description string. Never NULL.
 *
 * @note The returned string is statically allocated and must not be freed.
 *       The strings are locale-independent and English-only.
 * @thread_safety Safe to call from multiple threads concurrently.
 */
const char *argparse_error_string(argparse_error_code code);

/**
 * @brief Get the error message from a parse result.
 *
 * Returns the detailed error message describing why parsing failed.
 * If the result indicates success, this returns NULL.
 *
 * @param[in] result The parse result to query. Must not be NULL.
 * @return Pointer to the error message string, or NULL if no error occurred.
 *
 * @note The returned string is owned by the result and valid for its lifetime.
 * @thread_safety Safe to call from multiple threads concurrently, provided
 *                the result is not being modified concurrently.
 */
const char *argparse_result_error(const struct argparse_result *result);

/**
 * @brief Get the error code from a parse result.
 *
 * Returns the structured error code indicating the category of failure.
 * If the result indicates success, returns ARGPARSE_OK.
 *
 * @param[in] result The parse result to query. Must not be NULL.
 * @return The error code for this result.
 *
 * @thread_safety Safe to call from multiple threads concurrently, provided
 *                the result is not being modified concurrently.
 */
argparse_error_code argparse_result_error_code(const struct argparse_result *result);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_ERROR_H */
