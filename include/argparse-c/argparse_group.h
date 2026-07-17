/**
 * @file argparse_group.h
 * @brief Argument grouping for organized help output.
 *
 * Allows grouping related options together in help output with a
 * title and optional description.
 */

#ifndef ARGPARSE_GROUP_H
#define ARGPARSE_GROUP_H

#include "argparse_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a new argument group.
 *
 * Groups provide logical organization in help output. Options assigned
 * to a group are displayed together under the group's title.
 *
 * @param[in,out] parser      The parser to add the group to. Must not be NULL.
 * @param[in] title           The group title shown in help output. Must not
 *                            be NULL.
 * @param[in] description     Optional description shown below the title.
 *                            May be NULL.
 *
 * @return Pointer to the new group, or NULL on allocation failure.
 *
 * @note The group is owned by the parser and freed with argparse_free().
 * @note The returned pointer is valid for the lifetime of the parser.
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
struct argparse_group *argparse_add_group(struct argparse *parser,
                                          const char *title,
                                          const char *description);

/**
 * @brief Add an option to a group.
 *
 * Associates an option with a group so it appears under that group's
 * heading in help output.
 *
 * @param[in,out] group  The group to add the option to. Must not be NULL.
 * @param[in,out] option The option to add. Must not be NULL.
 *
 * @note The option must have been created with argparse_add_option() or
 *       argparse_add_positional().
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_group_add_option(struct argparse_group *group,
                               struct argparse_option *option);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_GROUP_H */
