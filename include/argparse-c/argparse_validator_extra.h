/**
 * @file argparse_validator_extra.h
 * @brief Built-in validators for range checking and pattern matching.
 *
 * Provides ready-to-use validation functions that can be attached to options
 * via argparse_option_set_validator(), as well as helper functions to
 * configure range constraints on integer and float options.
 */

#ifndef ARGPARSE_VALIDATOR_EXTRA_H
#define ARGPARSE_VALIDATOR_EXTRA_H

#include "argparse_types.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Range validator for integers: checks min <= value <= max.
 *
 * The min and max bounds are read from the option's range_min_int and
 * range_max_int fields, which must be set via argparse_option_set_range_int().
 *
 * @param[in] value           The string value to validate (parsed as int).
 * @param[out] error_buf      Buffer to receive the error message on failure.
 * @param[in] error_buf_size  Size of the error buffer in bytes.
 *
 * @return true if the value is within range, false otherwise.
 */
bool argparse_validate_int_range(const char *value, char *error_buf,
                                 size_t error_buf_size);

/**
 * @brief Range validator for floats: checks min <= value <= max.
 *
 * The min and max bounds are read from the option's range_min_float and
 * range_max_float fields, which must be set via argparse_option_set_range_float().
 *
 * @param[in] value           The string value to validate (parsed as float).
 * @param[out] error_buf      Buffer to receive the error message on failure.
 * @param[in] error_buf_size  Size of the error buffer in bytes.
 *
 * @return true if the value is within range, false otherwise.
 */
bool argparse_validate_float_range(const char *value, char *error_buf,
                                   size_t error_buf_size);

/**
 * @brief Pattern validator: simple glob-style matching.
 *
 * Supports '?' (matches any single character) and '*' (matches any sequence
 * of characters, including empty). The pattern is read from the option's
 * pattern field, which must be set via argparse_option_set_pattern().
 *
 * @param[in] value           The string value to validate.
 * @param[out] error_buf      Buffer to receive the error message on failure.
 * @param[in] error_buf_size  Size of the error buffer in bytes.
 *
 * @return true if the value matches the pattern, false otherwise.
 */
bool argparse_validate_pattern(const char *value, char *error_buf,
                               size_t error_buf_size);

/**
 * @brief Non-empty string validator.
 *
 * Checks that the value is not NULL and not an empty string.
 *
 * @param[in] value           The string value to validate.
 * @param[out] error_buf      Buffer to receive the error message on failure.
 * @param[in] error_buf_size  Size of the error buffer in bytes.
 *
 * @return true if the value is non-empty, false otherwise.
 */
bool argparse_validate_nonempty(const char *value, char *error_buf,
                                size_t error_buf_size);

/**
 * @brief Set integer range bounds on an option.
 *
 * Configures the option with the given min/max bounds and sets the
 * validator to argparse_validate_int_range.
 *
 * @param[in,out] option  The option to configure. Must not be NULL.
 * @param[in] min_val     Minimum allowed value (inclusive).
 * @param[in] max_val     Maximum allowed value (inclusive).
 */
void argparse_option_set_range_int(struct argparse_option *option,
                                   int min_val, int max_val);

/**
 * @brief Set float range bounds on an option.
 *
 * Configures the option with the given min/max bounds and sets the
 * validator to argparse_validate_float_range.
 *
 * @param[in,out] option  The option to configure. Must not be NULL.
 * @param[in] min_val     Minimum allowed value (inclusive).
 * @param[in] max_val     Maximum allowed value (inclusive).
 */
void argparse_option_set_range_float(struct argparse_option *option,
                                     float min_val, float max_val);

/**
 * @brief Set a glob-style pattern on an option.
 *
 * Configures the option with the given pattern and sets the validator
 * to argparse_validate_pattern.
 *
 * @param[in,out] option   The option to configure. Must not be NULL.
 * @param[in] pattern      Glob pattern string (supports '?' and '*').
 */
void argparse_option_set_pattern(struct argparse_option *option,
                                 const char *pattern);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_VALIDATOR_EXTRA_H */
