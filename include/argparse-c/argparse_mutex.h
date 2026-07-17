/**
 * @file argparse_mutex.h
 * @brief Mutually exclusive option groups.
 *
 * Defines groups of options where at most one may be specified on the
 * command line. If more than one is provided, parsing fails with an error.
 */

#ifndef ARGPARSE_MUTEX_H
#define ARGPARSE_MUTEX_H

#include "argparse_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a new mutually exclusive option group.
 *
 * At most one option in a mutually exclusive group may be specified on
 * the command line. Providing more than one results in a parse error.
 *
 * @param[in,out] parser The parser to add the mutex group to. Must not be NULL.
 * @param[in] group      Optional display group for these options in help
 *                       output. May be NULL.
 *
 * @return Pointer to the new mutex group, or NULL on allocation failure.
 *
 * @note The mutex group is owned by the parser and freed with argparse_free().
 * @note The returned pointer is valid for the lifetime of the parser.
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
struct argparse_mutex *argparse_add_mutex(struct argparse *parser,
                                          struct argparse_group *group);

/**
 * @brief Add an option to a mutually exclusive group.
 *
 * The option becomes part of the mutual exclusion constraint.
 *
 * @param[in,out] mutex  The mutex group to add the option to. Must not be NULL.
 * @param[in,out] option The option to add. Must not be NULL.
 *
 * @note The option must have been created with argparse_add_option().
 * @note The same option should not be added to multiple mutex groups.
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_mutex_add_option(struct argparse_mutex *mutex,
                               struct argparse_option *option);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_MUTEX_H */
