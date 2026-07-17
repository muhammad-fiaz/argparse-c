#include "argparse_internal.h"
#include "argparse-c/argparse_version.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static int action_store(struct argparse_option *opt, const char *value,
                        struct argparse_result *result);
static int action_store_const(struct argparse_option *opt,
                              struct argparse_result *result);
static int action_store_true(struct argparse_option *opt,
                             struct argparse_result *result);
static int action_store_false(struct argparse_option *opt,
                              struct argparse_result *result);
static int action_append(struct argparse_option *opt, const char *value,
                         struct argparse_result *result);
static int action_count(struct argparse_option *opt,
                        struct argparse_result *result);
static int action_help(struct argparse_option *opt,
                       struct argparse_result *result);
static int action_version(struct argparse_option *opt,
                          struct argparse_result *result);
static int action_extend(struct argparse_option *opt, const char *value,
                         struct argparse_result *result);

int argparse_action_perform(struct argparse_option *opt, const char *value,
                             struct argparse_result *result)
{
    if (!opt || !result) return -1;

    switch (opt->action) {
    case ARGPARSE_STORE:
        return action_store(opt, value, result);
    case ARGPARSE_STORE_CONST:
        return action_store_const(opt, result);
    case ARGPARSE_STORE_TRUE:
        return action_store_true(opt, result);
    case ARGPARSE_STORE_FALSE:
        return action_store_false(opt, result);
    case ARGPARSE_APPEND:
        return action_append(opt, value, result);
    case ARGPARSE_COUNT:
        return action_count(opt, result);
    case ARGPARSE_HELP:
        return action_help(opt, result);
    case ARGPARSE_VERSION:
        return action_version(opt, result);
    case ARGPARSE_EXTEND:
        return action_extend(opt, value, result);
    default:
        return -1;
    }
}

static const char *opt_dest(const struct argparse_option *opt)
{
    if (opt->dest) return opt->dest;
    if (opt->long_name) return opt->long_name;
    return "";
}

static int validate_value(struct argparse_option *opt, const char *value,
                          struct argparse_result *result)
{
    const argparse_allocator *alloc = result->parser ? result->parser->allocator : NULL;

    if (!value) return 0;

    if (opt->choices.count > 0) {
        bool found = false;
        for (size_t i = 0; i < opt->choices.count; i++) {
            if (argparse_streq(opt->choices.items[i], value)) {
                found = true;
                break;
            }
        }
        if (!found) {
            char msg[512];
            snprintf(msg, sizeof(msg), "invalid choice '%s' (choose from", value);
            size_t pos = strlen(msg);
            for (size_t i = 0; i < opt->choices.count && pos < sizeof(msg) - 2; i++) {
                if (i > 0) {
                    msg[pos++] = ',';
                    msg[pos++] = ' ';
                }
                const char *ch = opt->choices.items[i];
                size_t ch_len = strlen(ch);
                if (pos + ch_len + 2 < sizeof(msg)) {
                    memcpy(msg + pos, ch, ch_len);
                    pos += ch_len;
                }
            }
            msg[pos++] = ')';
            msg[pos] = '\0';
            argparse_set_error(result, ARGPARSE_ERROR_INVALID_VALUE, msg, alloc);
            return -1;
        }
    }

    if (opt->validator) {
        char error_buf[256] = {0};
        argparse_set_validator_context(opt);
        if (!opt->validator(value, error_buf, sizeof(error_buf))) {
            argparse_set_validator_context(NULL);
            argparse_set_error(result, ARGPARSE_ERROR_INVALID_VALUE,
                                error_buf[0] ? error_buf : "validation failed", alloc);
            return -1;
        }
        argparse_set_validator_context(NULL);
    }

    return 0;
}

static int action_store(struct argparse_option *opt, const char *value,
                        struct argparse_result *result)
{
    if (!opt || !result) return -1;
    const argparse_allocator *alloc = result->parser ? result->parser->allocator : NULL;

    if (validate_value(opt, value, result) != 0) {
        return -1;
    }

    argparse_value *sv = argparse_result_get_or_create(result, opt_dest(opt));
    if (!sv) return -1;

    switch (opt->type) {
    case ARGPARSE_TYPE_STRING:
    case ARGPARSE_TYPE_ENUM:
        argparse_free_internal(sv->string_value, alloc);
        sv->string_value = argparse_strdup(value, alloc);
        if (!sv->string_value && value) {
            argparse_set_error(result, ARGPARSE_ERROR_OUT_OF_MEMORY, NULL, alloc);
            return -1;
        }
        break;
    case ARGPARSE_TYPE_INT: {
        if (value) {
            char *end = NULL;
            errno = 0;
            long lv = strtol(value, &end, 10);
            if (errno != 0 || end == value || *end != '\0') {
                char msg[256];
                snprintf(msg, sizeof(msg), "invalid integer value '%s'", value);
                argparse_set_error(result, ARGPARSE_ERROR_INVALID_VALUE, msg, alloc);
                return -1;
            }
            sv->int_value = (int)lv;
        }
        break;
    }
    case ARGPARSE_TYPE_UINT: {
        if (value) {
            if (value[0] == '-') {
                char msg[256];
                snprintf(msg, sizeof(msg), "invalid unsigned integer value '%s'", value);
                argparse_set_error(result, ARGPARSE_ERROR_INVALID_VALUE, msg, alloc);
                return -1;
            }
            char *end = NULL;
            errno = 0;
            unsigned long uv = strtoul(value, &end, 10);
            if (errno != 0 || end == value || *end != '\0') {
                char msg[256];
                snprintf(msg, sizeof(msg), "invalid unsigned integer value '%s'", value);
                argparse_set_error(result, ARGPARSE_ERROR_INVALID_VALUE, msg, alloc);
                return -1;
            }
            sv->uint_value = (unsigned int)uv;
        }
        break;
    }
    case ARGPARSE_TYPE_FLOAT: {
        if (value) {
            char *end = NULL;
            errno = 0;
            double dv = strtod(value, &end);
            if (errno != 0 || end == value || *end != '\0') {
                char msg[256];
                snprintf(msg, sizeof(msg), "invalid float value '%s'", value);
                argparse_set_error(result, ARGPARSE_ERROR_INVALID_VALUE, msg, alloc);
                return -1;
            }
            sv->float_value = (float)dv;
        }
        break;
    }
    case ARGPARSE_TYPE_DOUBLE: {
        if (value) {
            char *end = NULL;
            errno = 0;
            double dv = strtod(value, &end);
            if (errno != 0 || end == value || *end != '\0') {
                char msg[256];
                snprintf(msg, sizeof(msg), "invalid double value '%s'", value);
                argparse_set_error(result, ARGPARSE_ERROR_INVALID_VALUE, msg, alloc);
                return -1;
            }
            sv->double_value = dv;
        }
        break;
    }
    case ARGPARSE_TYPE_BOOL:
        if (value) {
            sv->bool_value = (argparse_streq(value, "true") ||
                              argparse_streq(value, "1") ||
                              argparse_streq(value, "yes") ||
                              argparse_streq(value, "on"));
            argparse_free_internal(sv->string_value, alloc);
            sv->string_value = argparse_strdup(value, alloc);
        } else {
            sv->bool_value = true;
        }
        break;
    default:
        argparse_free_internal(sv->string_value, alloc);
        sv->string_value = argparse_strdup(value, alloc);
        break;
    }

    sv->is_set = true;

    if (opt->callback) {
        opt->callback(result->parser, opt->callback_data);
    }

    return 0;
}

static int action_store_const(struct argparse_option *opt,
                              struct argparse_result *result)
{
    return action_store(opt, opt->const_value, result);
}

static int action_store_true(struct argparse_option *opt,
                             struct argparse_result *result)
{
    if (!opt || !result) return -1;
    const argparse_allocator *alloc = result->parser ? result->parser->allocator : NULL;

    argparse_value *sv = argparse_result_get_or_create(result, opt_dest(opt));
    if (!sv) return -1;

    sv->bool_value = true;
    sv->int_value = 1;
    argparse_free_internal(sv->string_value, alloc);
    sv->string_value = argparse_strdup("true", alloc);
    sv->is_set = true;

    if (opt->callback) {
        opt->callback(result->parser, opt->callback_data);
    }
    return 0;
}

static int action_store_false(struct argparse_option *opt,
                              struct argparse_result *result)
{
    if (!opt || !result) return -1;
    const argparse_allocator *alloc = result->parser ? result->parser->allocator : NULL;

    argparse_value *sv = argparse_result_get_or_create(result, opt_dest(opt));
    if (!sv) return -1;

    sv->bool_value = false;
    sv->int_value = 0;
    argparse_free_internal(sv->string_value, alloc);
    sv->string_value = argparse_strdup("false", alloc);
    sv->is_set = true;

    if (opt->callback) {
        opt->callback(result->parser, opt->callback_data);
    }
    return 0;
}

static int action_append(struct argparse_option *opt, const char *value,
                         struct argparse_result *result)
{
    if (!opt || !result) return -1;
    const argparse_allocator *alloc = result->parser ? result->parser->allocator : NULL;

    if (validate_value(opt, value, result) != 0) {
        return -1;
    }

    argparse_value *sv = argparse_result_get_or_create(result, opt_dest(opt));
    if (!sv) return -1;

    if (!value) return 0;

    if (!sv->array_values) {
        sv->array_capacity = 4;
        sv->array_values = (char **)argparse_calloc(sv->array_capacity, sizeof(char *), alloc);
        if (!sv->array_values) return -1;
    }

    if (sv->array_count >= sv->array_capacity) {
        size_t new_cap = sv->array_capacity * 2;
        char **new_arr = (char **)argparse_realloc(
            sv->array_values,
            sv->array_capacity * sizeof(char *),
            new_cap * sizeof(char *), alloc);
        if (!new_arr) return -1;
        sv->array_values = new_arr;
        sv->array_capacity = new_cap;
    }

    sv->array_values[sv->array_count] = argparse_strdup(value, alloc);
    sv->array_count++;
    sv->is_set = true;

    if (opt->callback) {
        opt->callback(result->parser, opt->callback_data);
    }

    return 0;
}

static int action_count(struct argparse_option *opt,
                        struct argparse_result *result)
{
    if (!opt || !result) return -1;

    argparse_value *sv = argparse_result_get_or_create(result, opt_dest(opt));
    if (!sv) return -1;

    sv->int_value++;
    sv->is_set = true;

    if (opt->callback) {
        opt->callback(result->parser, opt->callback_data);
    }

    return 0;
}

static int action_help(struct argparse_option *opt,
                       struct argparse_result *result)
{
    (void)opt;
    if (!result || !result->parser) return -1;
    result->error_code = ARGPARSE_OK;
    return 1;
}

static int action_version(struct argparse_option *opt,
                          struct argparse_result *result)
{
    (void)opt;
    if (!result || !result->parser) return -1;
    printf("%s\n", argparse_version_string());
    result->error_code = ARGPARSE_OK;
    return 2;
}

static int action_extend(struct argparse_option *opt, const char *value,
                         struct argparse_result *result)
{
    return action_append(opt, value, result);
}
