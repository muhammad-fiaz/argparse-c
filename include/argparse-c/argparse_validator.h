/**
 * @file argparse_validator.h
 * @brief Custom value validators for options.
 *
 * Allows attaching custom validation functions to options that are called
 * after the value is parsed but before it is stored in the result namespace.
 */

#ifndef ARGPARSE_VALIDATOR_H
#define ARGPARSE_VALIDATOR_H

#include "argparse_types.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Custom validation function type.
 *
 * Called after the value is parsed to perform additional validation.
 * If validation fails, the function should write a descriptive error
 * message to the error buffer.
 *
 * @param[in] value           The string value to validate. Must not be NULL.
 * @param[out] error_buf      Buffer to receive the error message on failure.
 * @param[in] error_buf_size  Size of the error buffer in bytes.
 *
 * @return true if the value is valid, false if validation failed.
 *
 * @note The value has already been type-converted (e.g., to int) but is
 *       passed as a string for flexibility.
 */
typedef bool (*argparse_validate_fn)(const char *value,
                                     char *error_buf,
                                     size_t error_buf_size);

/**
 * @brief Set a custom validator on an option.
 *
 * The validator is called after type conversion and choice checking.
 * If the validator returns false, the parse is aborted with the error
 * message written to error_buf.
 *
 * @param[in,out] option    The option to configure. Must not be NULL.
 * @param[in] validator     The validation function. May be NULL to clear
 *                          an existing validator.
 *
 * @thread_safety Not thread-safe. The parser must not be accessed by
 *                other threads during this call.
 */
void argparse_option_set_validator(struct argparse_option *option,
                                   argparse_validate_fn validator);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_VALIDATOR_H */
