#include "argparse_internal.h"

#include <stdlib.h>
#include <string.h>

struct argparse_result *argparse_result_new(struct argparse *parser)
{
    if (!parser) return NULL;

    const argparse_allocator *a = parser->allocator;
    struct argparse_result *result = (struct argparse_result *)argparse_calloc(
        1, sizeof(*result), a);
    if (!result) return NULL;

    result->parser = parser;
    result->error_code = ARGPARSE_OK;
    result->value_capacity = 16;
    result->values = (argparse_value *)argparse_calloc(
        result->value_capacity, sizeof(argparse_value), a);

    if (!result->values) {
        argparse_free_internal(result, a);
        return NULL;
    }

    return result;
}

argparse_value *argparse_result_find(struct argparse_result *result, const char *key)
{
    if (!result || !key) return NULL;

    for (size_t i = 0; i < result->value_count; i++) {
        if (result->values[i].key && argparse_streq(result->values[i].key, key)) {
            return &result->values[i];
        }
    }
    return NULL;
}

argparse_value *argparse_result_get_or_create(struct argparse_result *result,
                                                 const char *key)
{
    if (!result || !key) return NULL;

    argparse_value *val = argparse_result_find(result, key);
    if (val) return val;

    const argparse_allocator *a = result->parser ? result->parser->allocator : NULL;

    if (result->value_count >= result->value_capacity) {
        size_t new_cap = result->value_capacity * 2;
        argparse_value *new_vals = (argparse_value *)argparse_realloc(
            result->values,
            result->value_capacity * sizeof(argparse_value),
            new_cap * sizeof(argparse_value), a);
        if (!new_vals) return NULL;
        result->values = new_vals;
        result->value_capacity = new_cap;
    }

    val = &result->values[result->value_count];
    memset(val, 0, sizeof(*val));
    val->key = argparse_strdup(key, a);
    if (!val->key) return NULL;

    result->value_count++;
    return val;
}

void argparse_result_free(struct argparse_result *result)
{
    argparse_free_result(result);
}

const char *argparse_get_string(const struct argparse_result *result, const char *dest)
{
    if (!result || !dest) return NULL;
    const argparse_value *val = argparse_result_find((struct argparse_result *)result, dest);
    if (!val || !val->is_set) return NULL;
    return val->string_value;
}

int argparse_get_int(const struct argparse_result *result, const char *dest)
{
    if (!result || !dest) return 0;
    const argparse_value *val = argparse_result_find((struct argparse_result *)result, dest);
    if (!val || !val->is_set) return 0;
    return val->int_value;
}

unsigned int argparse_get_uint(const struct argparse_result *result, const char *dest)
{
    if (!result || !dest) return 0;
    const argparse_value *val = argparse_result_find((struct argparse_result *)result, dest);
    if (!val || !val->is_set) return 0;
    return val->uint_value;
}

float argparse_get_float(const struct argparse_result *result, const char *dest)
{
    if (!result || !dest) return 0.0f;
    const argparse_value *val = argparse_result_find((struct argparse_result *)result, dest);
    if (!val || !val->is_set) return 0.0f;
    return val->float_value;
}

double argparse_get_double(const struct argparse_result *result, const char *dest)
{
    if (!result || !dest) return 0.0;
    const argparse_value *val = argparse_result_find((struct argparse_result *)result, dest);
    if (!val || !val->is_set) return 0.0;
    return val->double_value;
}

bool argparse_get_bool(const struct argparse_result *result, const char *dest)
{
    if (!result || !dest) return false;
    const argparse_value *val = argparse_result_find((struct argparse_result *)result, dest);
    if (!val || !val->is_set) return false;
    return val->bool_value;
}

const char *argparse_get_enum(const struct argparse_result *result, const char *dest)
{
    return argparse_get_string(result, dest);
}

size_t argparse_get_count(const struct argparse_result *result, const char *dest)
{
    if (!result || !dest) return 0;
    const argparse_value *val = argparse_result_find((struct argparse_result *)result, dest);
    if (!val || !val->is_set) return 0;
    if (val->array_values) return val->array_count;
    return val->string_value ? 1 : 0;
}

const char *argparse_get_string_at(const struct argparse_result *result,
                                    const char *dest, size_t index)
{
    if (!result || !dest) return NULL;
    const argparse_value *val = argparse_result_find((struct argparse_result *)result, dest);
    if (!val || !val->is_set) return NULL;
    if (val->array_values && index < val->array_count) {
        return val->array_values[index];
    }
    if (index == 0 && val->string_value) {
        return val->string_value;
    }
    return NULL;
}

int argparse_get_int_at(const struct argparse_result *result,
                         const char *dest, size_t index)
{
    const char *str = argparse_get_string_at(result, dest, index);
    if (!str) return 0;
    return atoi(str);
}
