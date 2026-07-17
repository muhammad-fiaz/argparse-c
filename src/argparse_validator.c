#include "argparse_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

static bool validate_int(const char *value, char *error_buf, size_t error_buf_size)
{
    if (!value) {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "value cannot be NULL");
        return false;
    }
    char *endptr = NULL;
    errno = 0;
    long val = strtol(value, &endptr, 10);
    if (errno != 0 || *endptr != '\0' || endptr == value) {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "not a valid integer");
        return false;
    }
    if (val < INT_MIN || val > INT_MAX) {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "integer out of range");
        return false;
    }
    return true;
}

static bool validate_uint(const char *value, char *error_buf, size_t error_buf_size)
{
    if (!value) {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "value cannot be NULL");
        return false;
    }
    char *endptr = NULL;
    errno = 0;
    unsigned long val = strtoul(value, &endptr, 10);
    if (errno != 0 || *endptr != '\0' || endptr == value) {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "not a valid unsigned integer");
        return false;
    }
    if (val > UINT_MAX) {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "unsigned integer out of range");
        return false;
    }
    return true;
}

static bool validate_float(const char *value, char *error_buf, size_t error_buf_size)
{
    if (!value) {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "value cannot be NULL");
        return false;
    }
    char *endptr = NULL;
    errno = 0;
    strtof(value, &endptr);
    if (errno != 0 || *endptr != '\0' || endptr == value) {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "not a valid float");
        return false;
    }
    return true;
}

static bool validate_double(const char *value, char *error_buf, size_t error_buf_size)
{
    if (!value) {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "value cannot be NULL");
        return false;
    }
    char *endptr = NULL;
    errno = 0;
    strtod(value, &endptr);
    if (errno != 0 || *endptr != '\0' || endptr == value) {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "not a valid double");
        return false;
    }
    return true;
}

static bool validate_bool(const char *value, char *error_buf, size_t error_buf_size)
{
    if (!value) {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "value cannot be NULL");
        return false;
    }
    if (argparse_streq(value, "true") || argparse_streq(value, "1") ||
        argparse_streq(value, "yes") || argparse_streq(value, "on") ||
        argparse_streq(value, "false") || argparse_streq(value, "0") ||
        argparse_streq(value, "no") || argparse_streq(value, "off")) {
        return true;
    }
    if (error_buf && error_buf_size > 0)
        snprintf(error_buf, error_buf_size, "not a valid boolean (use true/false, yes/no, 1/0)");
    return false;
}

argparse_validate_fn argparse_get_type_validator(argparse_type type)
{
    switch (type) {
    case ARGPARSE_TYPE_INT:    return validate_int;
    case ARGPARSE_TYPE_UINT:   return validate_uint;
    case ARGPARSE_TYPE_FLOAT:  return validate_float;
    case ARGPARSE_TYPE_DOUBLE: return validate_double;
    case ARGPARSE_TYPE_BOOL:   return validate_bool;
    default: return NULL;
    }
}
