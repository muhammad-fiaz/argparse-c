/**
 * @file argparse_lazy.h
 * @brief Lazy default value evaluation for options.
 *
 * Allows attaching a function that computes a default value on demand,
 * rather than specifying a static default at option creation time.
 * This is useful when the default depends on runtime state (e.g.,
 * environment variables, current directory, or other options).
 */

#ifndef ARGPARSE_LAZY_H
#define ARGPARSE_LAZY_H

#include "argparse_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Set a lazy default value function on an option.
 *
 * The function is called during apply_defaults (after parsing) for any
 * option that was not explicitly set on the command line. The returned
 * string is duplicated and used as the default value.
 *
 * If both a static default_value and a lazy_default_fn are set, the
 * lazy function takes precedence when the option is not set.
 *
 * @param[in,out] option    The option to configure. Must not be NULL.
 * @param[in] fn            The lazy default function. May be NULL to clear.
 * @param[in] user_data     Opaque pointer passed to fn on each invocation.
 */
void argparse_option_set_lazy_default(struct argparse_option *option,
                                      argparse_lazy_default_fn fn,
                                      void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_LAZY_H */
