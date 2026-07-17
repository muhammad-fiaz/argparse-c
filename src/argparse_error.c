#include "argparse_internal.h"

static const char *error_strings[] = {
    "success",
    "unknown error",
    "missing required argument",
    "unexpected argument",
    "invalid value",
    "too many arguments",
    "ambiguous option",
    "conflicting options",
    "out of memory",
    "invalid configuration"
};

#define ERROR_COUNT (sizeof(error_strings) / sizeof(error_strings[0]))

const char *argparse_error_string(argparse_error_code code)
{
    if ((size_t)code < ERROR_COUNT) {
        return error_strings[code];
    }
    return "unknown error";
}

void argparse_set_error(struct argparse_result *result, argparse_error_code code,
                         const char *message, const argparse_allocator *alloc)
{
    if (!result) return;
    argparse_free_internal(result->error_message, alloc);
    result->error_code = code;
    if (message) {
        result->error_message = argparse_strdup(message, alloc);
    } else {
        result->error_message = argparse_strdup(argparse_error_string(code), alloc);
    }
}

const char *argparse_result_error(const struct argparse_result *result)
{
    if (!result) {
        return NULL;
    }
    return result->error_message;
}

argparse_error_code argparse_result_error_code(const struct argparse_result *result)
{
    if (!result) {
        return ARGPARSE_ERROR_UNKNOWN;
    }
    return result->error_code;
}
