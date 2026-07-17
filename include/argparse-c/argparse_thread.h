/**
 * @file argparse_thread.h
 * @brief Thread-safe parsing support for the argparse-c library.
 *
 * Provides functions to freeze a parser configuration and perform
 * thread-safe parsing by documenting that frozen parsers can be used
 * concurrently by multiple threads for read-only parsing operations.
 */

#ifndef ARGPARSE_THREAD_H
#define ARGPARSE_THREAD_H

#include "argparse_types.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Mark a parser as frozen (read-only).
 *
 * Once frozen, the parser configuration must not be modified. This
 * enables safe concurrent use of the parser across multiple threads
 * for parsing operations.
 *
 * @param[in,out] parser The parser to freeze. Must not be NULL.
 *
 * @thread_safety This function is not thread-safe. It should be called
 *                before any concurrent parsing begins.
 */
void argparse_freeze(struct argparse *parser);

/**
 * @brief Check if a parser is frozen (read-only).
 *
 * @param[in] parser The parser to query. Must not be NULL.
 *
 * @return true if the parser is frozen, false otherwise.
 *
 * @thread_safety Safe to call concurrently on the same parser.
 */
bool argparse_is_frozen(const struct argparse *parser);

/**
 * @brief Thread-safe parse: parse arguments using a frozen parser.
 *
 * Creates a parse result from the given arguments using the parser's
 * configuration. The parser must be frozen (via argparse_freeze()) before
 * calling this function. Multiple threads may call this function
 * concurrently on the same frozen parser, each receiving independent
 * results.
 *
 * @param[in] parser The frozen parser to use. Must not be NULL and must
 *                   be frozen.
 * @param[in] argc   The argument count (typically from main).
 * @param[in] argv   The argument vector (typically from main). Must not
 *                   be NULL.
 *
 * @return A newly allocated parse result, or NULL on allocation failure.
 *         Check argparse_result_error_code() for the parse outcome.
 *
 * @note The caller is responsible for freeing the result with
 *       argparse_result_free().
 * @note This function behaves identically to argparse_parse() but
 *       documents the requirement that the parser be frozen for safe
 *       concurrent use.
 * @thread_safety Safe to call concurrently on the same frozen parser.
 *                Each call produces an independent result.
 */
struct argparse_result *argparse_parse_threadsafe(struct argparse *parser,
                                                   int argc,
                                                   const char **argv);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_THREAD_H */
