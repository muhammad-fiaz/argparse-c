/**
 * @file argparse_types.h
 * @brief Core type definitions, enumerations, and opaque forward declarations
 *        for the argparse-c library.
 *
 * This header defines all public enumerations, function pointer typedefs, and
 * opaque struct forward declarations used throughout the library API.
 */

#ifndef ARGPARSE_TYPES_H
#define ARGPARSE_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup enums Enumerations
 * @{
 */

/**
 * @brief Action to perform when an option is encountered.
 *
 * Determines how the parsed value is stored or what side effect occurs
 * when the option is matched on the command line.
 */
typedef enum argparse_action {
    /**
     * Store the value (or nargs values) into the dest variable.
     * This is the default action for most option types.
     */
    ARGPARSE_STORE = 0,

    /**
     * Store the const value (set via argparse_option_set_const) into dest.
     * Used for flag-style options that do not consume arguments.
     */
    ARGPARSE_STORE_CONST,

    /**
     * Equivalent to ARGPARSE_STORE with const_value = "true".
     * Sets dest to true when the flag is present.
     */
    ARGPARSE_STORE_TRUE,

    /**
     * Equivalent to ARGPARSE_STORE with const_value = "false".
     * Sets dest to false when the flag is present.
     */
    ARGPARSE_STORE_FALSE,

    /**
     * Append each value to a list in dest.
     * Supports repeated specification of the same option.
     */
    ARGPARSE_APPEND,

    /**
     * Increment an integer counter in dest each time the option is seen.
     * Useful for verbosity flags like -v, -vv, -vvv.
     */
    ARGPARSE_COUNT,

    /**
     * Print help information and exit.
     * No value is stored in dest.
     */
    ARGPARSE_HELP,

    /**
     * Print version information and exit.
     * No value is stored in dest.
     */
    ARGPARSE_VERSION,

    /**
     * Like ARGPARSE_APPEND, but extend the list with multiple values
     * from a single argument (e.g., --opt val1 val2 val3).
     */
    ARGPARSE_EXTEND
} argparse_action;

/**
 * @brief Type of value expected for an option or positional argument.
 *
 * Determines how the string value from the command line is parsed
 * and validated before being stored.
 */
typedef enum argparse_type {
    /** No type constraint (store as raw string). */
    ARGPARSE_TYPE_NONE = 0,

    /** String value (no conversion). */
    ARGPARSE_TYPE_STRING,

    /** Signed integer value. */
    ARGPARSE_TYPE_INT,

    /** Unsigned integer value. */
    ARGPARSE_TYPE_UINT,

    /** Single-precision floating-point value. */
    ARGPARSE_TYPE_FLOAT,

    /** Double-precision floating-point value. */
    ARGPARSE_TYPE_DOUBLE,

    /** Boolean value (true/false, yes/no, 1/0). */
    ARGPARSE_TYPE_BOOL,

    /** Enumeration value (one of a set of allowed string choices). */
    ARGPARSE_TYPE_ENUM
} argparse_type;

/**
 * @brief Number of arguments an option or positional expects.
 *
 * Controls how many command-line tokens are consumed by the option.
 * Similar to Python's argparse nargs parameter.
 */
typedef enum argparse_nargs {
    /** Zero arguments (flag option). */
    ARGPARSE_NARGS_0 = 0,

    /** Exactly one argument (default for most options). */
    ARGPARSE_NARGS_1 = 1,

    /** Exactly two arguments. */
    ARGPARSE_NARGS_2 = 2,

    /** Exactly three arguments. */
    ARGPARSE_NARGS_3 = 3,

    /**
     * Zero or one argument (the argument is optional).
     * Stored as NULL or nargs_1 if present.
     */
    ARGPARSE_NARGS_QUESTION = -1,

    /**
     * Zero or more arguments (greedy).
     * Collects all remaining arguments until the next option.
     */
    ARGPARSE_NARGS_STAR = -2,

    /**
     * One or more arguments (greedy).
     * Like STAR but at least one argument is required.
     */
    ARGPARSE_NARGS_PLUS = -3
} argparse_nargs;

/**
 * @brief Error code returned by parse operations and validation.
 */
typedef enum argparse_error_code {
    /** Operation completed successfully. */
    ARGPARSE_OK = 0,

    /** An unknown or internal error occurred. */
    ARGPARSE_ERROR_UNKNOWN,

    /** A required argument was not provided. */
    ARGPARSE_ERROR_MISSING_ARGUMENT,

    /** An argument was provided where none was expected. */
    ARGPARSE_ERROR_UNEXPECTED_ARGUMENT,

    /** The value could not be converted to the expected type. */
    ARGPARSE_ERROR_INVALID_VALUE,

    /** More arguments were provided than expected. */
    ARGPARSE_ERROR_TOO_MANY_ARGUMENTS,

    /** An option was ambiguous (matched multiple options). */
    ARGPARSE_ERROR_AMBIGUOUS_OPTION,

    /** Conflicting options were provided (e.g., mutually exclusive). */
    ARGPARSE_ERROR_CONFLICT,

    /** A memory allocation failed. */
    ARGPARSE_ERROR_OUT_OF_MEMORY,

    /** The parser configuration is invalid. */
    ARGPARSE_ERROR_INVALID_CONFIG
} argparse_error_code;

/**
 * @brief Destination type for storing parsed values.
 *
 * Determines whether the parsed value is stored in a named variable
 * or passed to a callback function.
 */
typedef enum argparse_dest_type {
    /** No destination configured. */
    ARGPARSE_DEST_NONE = 0,

    /** Store the value by name in the result namespace. */
    ARGPARSE_DEST_STRING,

    /** Invoke a callback function with the parsed value. */
    ARGPARSE_DEST_FUNC
} argparse_dest_type;

/** @} */ /* end of enums group */

/**
 * @defgroup forward_declarations Opaque Forward Declarations
 * @{
 */

/** @brief Opaque parser instance. Created via argparse_new(). */
struct argparse;

/** @brief Opaque parse result / namespace. Created via argparse_parse(). */
struct argparse_result;

/** @brief Opaque option definition. Created via argparse_add_option() or argparse_add_positional(). */
struct argparse_option;

/** @brief Opaque argument group. Created via argparse_add_group(). */
struct argparse_group;

/** @brief Opaque mutually exclusive group. Created via argparse_add_mutex(). */
struct argparse_mutex;

/** @brief Opaque subcommand. Created via argparse_add_command(). */
struct argparse_command;

/** @} */ /* end of forward_declarations group */

/**
 * @defgroup callbacks Function Pointer Types
 * @{
 */

/**
 * @brief Callback function type for option actions and subcommand handlers.
 *
 * @param[in,out] parser The parser instance (for accessing other options).
 * @param[in,out] data   User-defined data pointer provided when registering the callback.
 */
typedef void (*argparse_callback_fn)(struct argparse *parser, void *data);

/**
 * @brief Custom memory allocation function.
 *
 * @param[in] size      Number of bytes to allocate.
 * @param[in] user_data Opaque pointer passed through from the allocator.
 * @return Pointer to allocated memory, or NULL on failure.
 */
typedef void *(*argparse_alloc_fn)(size_t size, void *user_data);

/**
 * @brief Custom memory deallocation function.
 *
 * @param[in] ptr       Pointer to memory to free (may be NULL).
 * @param[in] user_data Opaque pointer passed through from the allocator.
 */
typedef void (*argparse_free_fn)(void *ptr, void *user_data);

/**
 * @brief Lazy default value function type.
 *
 * Called during apply_defaults to compute a default value on demand.
 * The function should return a string that is valid for the lifetime
 * of the call (the caller will duplicate it).
 *
 * @param[in] user_data  Opaque pointer provided when registering the function.
 *
 * @return A string to use as the default value, or NULL for no default.
 */
typedef const char *(*argparse_lazy_default_fn)(void *user_data);

/** @} */ /* end of callbacks group */

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_TYPES_H */
