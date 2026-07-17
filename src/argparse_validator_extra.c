#include "argparse_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

static struct argparse_option *current_option_context = NULL;

void argparse_set_validator_context(struct argparse_option *opt)
{
    current_option_context = opt;
}

struct argparse_option *argparse_get_validator_context(void)
{
    return current_option_context;
}

static bool glob_match(const char *pattern, const char *str)
{
    if (!pattern || !str) return false;

    while (*pattern && *str) {
        if (*pattern == '*') {
            pattern++;
            if (!*pattern) return true;
            while (*str) {
                if (glob_match(pattern, str)) return true;
                str++;
            }
            return glob_match(pattern, str);
        } else if (*pattern == '?' || *pattern == *str) {
            pattern++;
            str++;
        } else {
            return false;
        }
    }

    while (*pattern == '*') pattern++;
    return *pattern == '\0' && *str == '\0';
}

bool argparse_validate_int_range(const char *value, char *error_buf,
                                 size_t error_buf_size)
{
    if (!value) {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "value cannot be NULL");
        return false;
    }

    char *endptr = NULL;
    errno = 0;
    long lv = strtol(value, &endptr, 10);
    if (errno != 0 || *endptr != '\0' || endptr == value) {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "not a valid integer");
        return false;
    }
    if (lv < INT_MIN || lv > INT_MAX) {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "integer out of range");
        return false;
    }

    struct argparse_option *opt = argparse_get_validator_context();
    if (opt) {
        int iv = (int)lv;
        if (iv < opt->range_min_int || iv > opt->range_max_int) {
            if (error_buf && error_buf_size > 0)
                snprintf(error_buf, error_buf_size,
                         "value %d out of range [%d, %d]",
                         iv, opt->range_min_int, opt->range_max_int);
            return false;
        }
    }

    return true;
}

bool argparse_validate_float_range(const char *value, char *error_buf,
                                   size_t error_buf_size)
{
    if (!value) {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "value cannot be NULL");
        return false;
    }

    char *endptr = NULL;
    errno = 0;
    double dv = strtod(value, &endptr);
    if (errno != 0 || *endptr != '\0' || endptr == value) {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "not a valid float");
        return false;
    }

    struct argparse_option *opt = argparse_get_validator_context();
    if (opt) {
        float fv = (float)dv;
        if (fv < opt->range_min_float || fv > opt->range_max_float) {
            if (error_buf && error_buf_size > 0)
                snprintf(error_buf, error_buf_size,
                         "value %.6g out of range [%.6g, %.6g]",
                         (double)fv, (double)opt->range_min_float,
                         (double)opt->range_max_float);
            return false;
        }
    }

    return true;
}

bool argparse_validate_pattern(const char *value, char *error_buf,
                               size_t error_buf_size)
{
    if (!value) {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "value cannot be NULL");
        return false;
    }

    struct argparse_option *opt = argparse_get_validator_context();
    if (opt && opt->pattern) {
        if (!glob_match(opt->pattern, value)) {
            if (error_buf && error_buf_size > 0)
                snprintf(error_buf, error_buf_size,
                         "value '%s' does not match pattern '%s'",
                         value, opt->pattern);
            return false;
        }
    }

    return true;
}

bool argparse_validate_nonempty(const char *value, char *error_buf,
                                size_t error_buf_size)
{
    if (!value || *value == '\0') {
        if (error_buf && error_buf_size > 0)
            snprintf(error_buf, error_buf_size, "value must not be empty");
        return false;
    }
    return true;
}

void argparse_option_set_range_int(struct argparse_option *option,
                                   int min_val, int max_val)
{
    if (!option) return;
    option->range_min_int = min_val;
    option->range_max_int = max_val;
    option->validator = argparse_validate_int_range;
}

void argparse_option_set_range_float(struct argparse_option *option,
                                     float min_val, float max_val)
{
    if (!option) return;
    option->range_min_float = min_val;
    option->range_max_float = max_val;
    option->validator = argparse_validate_float_range;
}

void argparse_option_set_pattern(struct argparse_option *option,
                                 const char *pattern)
{
    if (!option) return;
    const argparse_allocator *a = NULL;
    argparse_free_internal(option->pattern, a);
    option->pattern = argparse_strdup(pattern, a);
    option->validator = argparse_validate_pattern;
}
