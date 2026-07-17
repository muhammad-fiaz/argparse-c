/**
 * @file argparse_manpage.h
 * @brief Man page generation in groff/troff format.
 *
 * Provides functions to generate Unix manual pages from an argparse parser
 * definition. The output follows standard groff/troff formatting with
 * proper sections (NAME, SYNOPSIS, DESCRIPTION, OPTIONS, EXAMPLES,
 * SEE ALSO, AUTHORS).
 */

#ifndef ARGPARSE_MANPAGE_H
#define ARGPARSE_MANPAGE_H

#include "argparse_types.h"

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Generate a man page in groff/troff format to a FILE stream.
 *
 * Writes a complete man page to the given output stream. The generated
 * page includes the following standard sections:
 * - **NAME**: Program name and short description.
 * - **SYNOPSIS**: Usage summary with options and positional arguments.
 * - **DESCRIPTION**: Full description of the program.
 * - **OPTIONS**: Detailed list of all optional arguments with their
 *   short flags, long flags, types, and help text.
 * - **EXAMPLES**: Placeholder section with common usage examples.
 * - **SEE ALSO**: Cross-references to related commands.
 * - **AUTHORS**: Program author attribution.
 *
 * @param[in] parser The parser to generate the man page for.
 *                    Must not be NULL.
 * @param[in] fp      Output FILE stream. Must not be NULL and must be
 *                    open for writing.
 *
 * @return 0 on success, -1 on failure (write error or NULL arguments).
 *
 * @note The output is suitable for viewing with @c man or @c groff -man.
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
int argparse_generate_manpage(const struct argparse *parser, FILE *fp);

/**
 * @brief Generate a man page and write it to a file.
 *
 * Convenience function that opens the specified file, writes a complete
 * groff/troff man page, and closes the file.
 *
 * @param[in] parser   The parser to generate the man page for.
 *                      Must not be NULL.
 * @param[in] filename Output file path. Must not be NULL. The file will
 *                     be created or overwritten.
 *
 * @return 0 on success, -1 on failure (file open error, write error,
 *         or NULL arguments).
 *
 * @note The generated file can be viewed with @c man ./filename.1.
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
int argparse_generate_manpage_file(const struct argparse *parser,
                                   const char *filename);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_MANPAGE_H */
