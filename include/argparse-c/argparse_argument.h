/**
 * @file argparse_argument.h
 * @brief Parser lifecycle, configuration, and option/argument creation.
 *
 * This is the primary header for creating parsers, adding options and
 * positional arguments, and configuring parser behavior. It provides the
 * core API similar to Python's argparse.ArgumentParser.
 */

#ifndef ARGPARSE_ARGUMENT_H
#define ARGPARSE_ARGUMENT_H

#include "argparse_types.h"

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup parser Parser Lifecycle
 * @{
 */

/**
 * @brief Create a new argument parser.
 *
 * Allocates and initializes a new parser instance with the given program
 * name and description. The parser starts with no options, no positional
 * arguments, and default settings.
 *
 * @param[in] prog        Program name for help/usage output. May be NULL
 *                        (defaults to "program").
 * @param[in] description Brief description shown in help output. May be NULL.
 *
 * @return Pointer to the new parser, or NULL on allocation failure.
 *
 * @note The caller is responsible for freeing the parser with argparse_free().
 * @thread_safety Each parser instance is NOT thread-safe. Use separate
 *                instances for concurrent use.
 */
struct argparse *argparse_new(const char *prog, const char *description);

/**
 * @brief Free a parser and all associated resources.
 *
 * Releases all memory held by the parser, including all options, groups,
 * commands, and internal state. After this call, the pointer is invalid.
 *
 * @param[in] parser The parser to free. May be NULL (no-op).
 *
 * @note Safe to call with NULL. Safe to call multiple times on the same parser
 *       (second and subsequent calls are no-ops).
 * @thread_safety The parser must not be accessed by other threads during
 *                or after this call.
 */
void argparse_free(struct argparse *parser);

/** @} */ /* end of parser group */

/**
 * @defgroup parser_config Parser Configuration
 * @{
 */

/**
 * @brief Set the program name displayed in help/usage output.
 *
 * @param[in,out] parser The parser to configure. Must not be NULL.
 * @param[in] prog       The program name string. Copied internally.
 *                       May be NULL to clear.
 *
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_set_prog(struct argparse *parser, const char *prog);

/**
 * @brief Set the description shown in help output after the usage line.
 *
 * @param[in,out] parser      The parser to configure. Must not be NULL.
 * @param[in] description     The description string. Copied internally.
 *                            May be NULL to clear.
 *
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_set_description(struct argparse *parser, const char *description);

/**
 * @brief Set the epilog shown in help output after the argument list.
 *
 * @param[in,out] parser The parser to configure. Must not be NULL.
 * @param[in] epilog     The epilog string. Copied internally.
 *                       May be NULL to clear.
 *
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_set_epilog(struct argparse *parser, const char *epilog);

/**
 * @brief Override the auto-generated usage line.
 *
 * When set, this string replaces the automatically generated usage summary
 * in help output.
 *
 * @param[in,out] parser The parser to configure. Must not be NULL.
 * @param[in] usage      The custom usage string. Copied internally.
 *                       May be NULL to restore auto-generation.
 *
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_set_usage(struct argparse *parser, const char *usage);

/**
 * @brief Set the characters that prefix optional arguments.
 *
 * By default, options are prefixed with '-'. This function allows changing
 * that prefix (e.g., to "/" on Windows).
 *
 * @param[in,out] parser The parser to configure. Must not be NULL.
 * @param[in] chars      String of prefix characters. Copied internally.
 *                       May be NULL to use defaults.
 *
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_set_prefix_chars(struct argparse *parser, const char *chars);

/**
 * @brief Set the characters that prefix from-file argument prefixes.
 *
 * When an argument starts with one of these characters, it is treated as
 * a filename whose contents are read and treated as additional arguments.
 *
 * @param[in,out] parser The parser to configure. Must not be NULL.
 * @param[in] chars      String of from-file prefix characters. Copied
 *                       internally. May be NULL to disable from-file support.
 *
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_set_fromfile_prefix_chars(struct argparse *parser, const char *chars);

/** @} */ /* end of parser_config group */

/**
 * @defgroup option_add Adding Options and Positional Arguments
 * @{
 */

/**
 * @brief Add an optional argument (option) to the parser.
 *
 * Creates a new option that can be specified on the command line with a
 * short flag (-x), a long flag (--example), or both. At least one of
 * short_name or long_name must be provided.
 *
 * @param[in,out] parser   The parser to add the option to. Must not be NULL.
 * @param[in] short_name   Single-character short option name (e.g., 'v'),
 *                         or 0 for none.
 * @param[in] long_name    Long option name without prefix (e.g., "verbose"),
 *                         or NULL for none.
 * @param[in] nargs        Number of arguments the option expects.
 * @param[in] type         Type of the value to parse.
 * @param[in] help         Help text for this option. May be NULL.
 * @param[in] dest         Destination name in the result namespace. May be
 *                         NULL (auto-generated from long_name).
 *
 * @return Pointer to the newly created option, or NULL on failure.
 *
 * @note At least one of short_name or long_name must be non-zero/non-NULL.
 * @note The returned pointer is valid for the lifetime of the parser.
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
struct argparse_option *argparse_add_option(struct argparse *parser,
                                            char short_name,
                                            const char *long_name,
                                            argparse_nargs nargs,
                                            argparse_type type,
                                            const char *help,
                                            const char *dest);

/**
 * @brief Add a positional argument to the parser.
 *
 * Positional arguments are identified by their position on the command line
 * rather than by flags. They are processed in the order they are added.
 *
 * @param[in,out] parser The parser to add the positional argument to.
 *                       Must not be NULL.
 * @param[in] nargs      Number of arguments this positional expects.
 * @param[in] type       Type of the value to parse.
 * @param[in] help       Help text for this positional. May be NULL.
 * @param[in] dest       Destination name in the result namespace. May be
 *                       NULL (auto-generated).
 *
 * @return Pointer to the newly created option, or NULL on failure.
 *
 * @note The returned pointer is valid for the lifetime of the parser.
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
struct argparse_option *argparse_add_positional(struct argparse *parser,
                                                argparse_nargs nargs,
                                                argparse_type type,
                                                const char *help,
                                                const char *dest);

/** @} */ /* end of option_add group */

/**
 * @defgroup option_config Option Configuration
 * @{
 */

/**
 * @brief Set the default value for an option.
 *
 * The default value is used when the option is not present on the command
 * line. The string is converted to the option's type during parsing.
 *
 * @param[in,out] option        The option to configure. Must not be NULL.
 * @param[in] default_value    The default value as a string. Copied internally.
 *                             May be NULL for no default.
 *
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_option_set_default(struct argparse_option *option,
                                 const char *default_value);

/**
 * @brief Set the constant value for store-const actions.
 *
 * Used with ARGPARSE_STORE_CONST, ARGPARSE_STORE_TRUE, and
 * ARGPARSE_STORE_FALSE actions. This is the value stored when the
 * option is present.
 *
 * @param[in,out] option     The option to configure. Must not be NULL.
 * @param[in] const_value    The constant value string. Copied internally.
 *                           May be NULL.
 *
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_option_set_const(struct argparse_option *option,
                               const char *const_value);

/**
 * @brief Set the allowed choices for an option.
 *
 * Restricts the option's value to one of the specified choices. When set,
 * the option's type is automatically set to ARGPARSE_TYPE_ENUM.
 *
 * @param[in,out] option   The option to configure. Must not be NULL.
 * @param[in] choices      Array of allowed choice strings. Copied internally.
 * @param[in] count        Number of choices in the array. Must be > 0.
 *
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_option_set_choices(struct argparse_option *option,
                                 const char **choices,
                                 size_t count);

/**
 * @brief Set whether an option is required.
 *
 * When required, parsing fails if the option is not provided on the
 * command line. Default is false (optional).
 *
 * @param[in,out] option  The option to configure. Must not be NULL.
 * @param[in] required    True if the option is required, false otherwise.
 *
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_option_set_required(struct argparse_option *option,
                                  bool required);

/**
 * @brief Set the metavar for an option in help output.
 *
 * The metavar is the placeholder shown in help text to represent the
 * option's value (e.g., "FILE" in "--output FILE"). If not set, the
 * destination name or type name is used.
 *
 * @param[in,out] option  The option to configure. Must not be NULL.
 * @param[in] metavar     The metavar string. Copied internally.
 *                        May be NULL for auto-generation.
 *
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_option_set_metavar(struct argparse_option *option,
                                 const char *metavar);

/**
 * @brief Set the action for an option.
 *
 * Overrides the default action (ARGPARSE_STORE) with a different action.
 *
 * @param[in,out] option  The option to configure. Must not be NULL.
 * @param[in] action      The desired action.
 *
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_option_set_action(struct argparse_option *option,
                                argparse_action action);

/**
 * @brief Set a callback function for an option.
 *
 * When the option is encountered during parsing, the callback is invoked
 * instead of (or in addition to) storing the value.
 *
 * @param[in,out] option   The option to configure. Must not be NULL.
 * @param[in] callback     The callback function. Must not be NULL.
 * @param[in] data         Opaque user data passed to the callback. May be NULL.
 *
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_option_set_callback(struct argparse_option *option,
                                  argparse_callback_fn callback,
                                  void *data);

/** @} */ /* end of option_config group */

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_ARGUMENT_H */
