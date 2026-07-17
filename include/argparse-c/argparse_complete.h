/**
 * @file argparse_complete.h
 * @brief Shell completion support.
 *
 * Provides functions to generate shell completion scripts for various
 * shells (bash, zsh, fish, PowerShell). These can be sourced by the
 * user's shell configuration to enable tab-completion for programs
 * using argparse-c.
 */

#ifndef ARGPARSE_COMPLETE_H
#define ARGPARSE_COMPLETE_H

#include "argparse_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Generate bash completion script to stdout.
 *
 * Outputs a complete bash completion function that can be sourced
 * in .bashrc or similar. The script introspects the parser's options
 * and subcommands to provide accurate completions.
 *
 * @param[in] parser The parser to generate completions for. Must not be NULL.
 * @param[in] argc   Current argument count (for runtime completion context).
 * @param[in] argv   Current argument vector (for runtime completion context).
 *
 * @note Output is written to stdout.
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_complete_bash(struct argparse *parser,
                            int argc,
                            const char **argv);

/**
 * @brief Generate zsh completion script to stdout.
 *
 * Outputs a complete zsh completion function with descriptions for each
 * option. The script provides rich completions including option descriptions
 * and subcommand support.
 *
 * @param[in] parser The parser to generate completions for. Must not be NULL.
 * @param[in] argc   Current argument count (for runtime completion context).
 * @param[in] argv   Current argument vector (for runtime completion context).
 *
 * @note Output is written to stdout.
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_complete_zsh(struct argparse *parser,
                           int argc,
                           const char **argv);

/**
 * @brief Generate fish completion script to stdout.
 *
 * Outputs a complete fish completion function. Fish uses a different
 * completion mechanism than bash/zsh, with subcommand-aware completions.
 *
 * @param[in] parser The parser to generate completions for. Must not be NULL.
 * @param[in] argc   Current argument count (for runtime completion context).
 * @param[in] argv   Current argument vector (for runtime completion context).
 *
 * @note Output is written to stdout.
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_complete_fish(struct argparse *parser,
                            int argc,
                            const char **argv);

/**
 * @brief Generate PowerShell completion script to stdout.
 *
 * Outputs a complete PowerShell tab-completion function. Supports
 * Windows PowerShell 5.1+ and PowerShell Core.
 *
 * @param[in] parser The parser to generate completions for. Must not be NULL.
 * @param[in] argc   Current argument count (for runtime completion context).
 * @param[in] argv   Current argument vector (for runtime completion context).
 *
 * @note Output is written to stdout.
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_complete_powershell(struct argparse *parser,
                                  int argc,
                                  const char **argv);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_COMPLETE_H */
