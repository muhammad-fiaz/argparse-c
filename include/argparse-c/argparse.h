/**
 * @file argparse.h
 * @brief Convenience header for the argparse-c library.
 *
 * Including this single header provides access to all public API
 * functions, types, and enumerations. This is the recommended way
 * to use the library.
 *
 * @code
 * #include <argparse-c/argparse.h>
 * @endcode
 *
 * @see argparse_types.h for core types and enumerations
 * @see argparse_argument.h for parser creation and option configuration
 * @see argparse_namespace.h for parsing and value access
 * @see argparse_format.h for help output
 * @see argparse_version.h for version information
 */

#ifndef ARGPARSE_H
#define ARGPARSE_H

#include "argparse_types.h"
#include "argparse_version.h"
#include "argparse_memory.h"
#include "argparse_error.h"
#include "argparse_argument.h"
#include "argparse_group.h"
#include "argparse_mutex.h"
#include "argparse_command.h"
#include "argparse_namespace.h"
#include "argparse_format.h"
#include "argparse_validator.h"
#include "argparse_validator_extra.h"
#include "argparse_color.h"
#include "argparse_lazy.h"
#include "argparse_complete.h"
#include "argparse_config.h"
#include "argparse_unicode.h"
#include "argparse_batch.h"
#include "argparse_thread.h"
#include "argparse_plugin.h"
#include "argparse_i18n.h"
#include "argparse_manpage.h"
#include "argparse_json.h"

#endif /* ARGPARSE_H */
