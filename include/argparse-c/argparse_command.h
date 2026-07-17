/**
 * @file argparse_command.h
 * @brief Subcommand support.
 *
 * Allows defining subcommands (like "git commit" or "git push") each
 * with their own parser, options, and callback handler.
 */

#ifndef ARGPARSE_COMMAND_H
#define ARGPARSE_COMMAND_H

#include "argparse_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Add a subcommand to the parser.
 *
 * Creates a new subcommand with its own dedicated parser. The subcommand's
 * parser can have its own options and positional arguments. When the
 * subcommand is matched, its callback is invoked.
 *
 * @param[in,out] parser   The parent parser. Must not be NULL.
 * @param[in] name         The subcommand name as it appears on the command
 *                         line. Must not be NULL.
 * @param[in] help         Help text shown for this subcommand. May be NULL.
 * @param[in] callback     Function called when this subcommand is selected.
 *                         May be NULL.
 *
 * @return Pointer to the new command, or NULL on allocation failure.
 *
 * @note The returned pointer is valid for the lifetime of the parser.
 * @note Use argparse_command_get_parser() to get the subcommand's parser
 *       for adding its own options.
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
struct argparse_command *argparse_add_command(struct argparse *parser,
                                              const char *name,
                                              const char *help,
                                              argparse_callback_fn callback);

/**
 * @brief Get the parser associated with a subcommand.
 *
 * Returns the subcommand's dedicated parser, which can be used to add
 * subcommand-specific options and positional arguments.
 *
 * @param[in] command The subcommand. Must not be NULL.
 *
 * @return Pointer to the subcommand's parser. Never NULL.
 *
 * @note The returned parser is owned by the command and freed with the
 *       parent parser. Do not free it separately.
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
struct argparse *argparse_command_get_parser(struct argparse_command *command);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_COMMAND_H */
