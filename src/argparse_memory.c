#include "argparse_internal.h"

#include <stdlib.h>
#include <string.h>

static void *argparse_default_alloc(size_t size, void *user_data)
{
    (void)user_data;
    return malloc(size);
}

static void argparse_default_free(void *ptr, void *user_data)
{
    (void)user_data;
    free(ptr);
}

static const argparse_allocator default_allocator = {
    argparse_default_alloc,
    argparse_default_free,
    NULL
};

static const argparse_allocator *global_default_allocator = &default_allocator;

const argparse_allocator *argparse_default_allocator(void)
{
    return global_default_allocator;
}

void argparse_set_default_allocator(const argparse_allocator *allocator)
{
    if (allocator) {
        global_default_allocator = allocator;
    } else {
        global_default_allocator = &default_allocator;
    }
}

const argparse_allocator *argparse_resolve_allocator(const argparse_allocator *alloc)
{
    if (alloc) {
        return alloc;
    }
    return global_default_allocator;
}

void *argparse_alloc(size_t size)
{
    const argparse_allocator *a = argparse_resolve_allocator(NULL);
    if (!a || !a->alloc) {
        return NULL;
    }
    return a->alloc(size, a->user_data);
}

void argparse_mem_free(void *ptr)
{
    if (!ptr) {
        return;
    }
    const argparse_allocator *a = argparse_resolve_allocator(NULL);
    if (!a || !a->free) {
        return;
    }
    a->free(ptr, a->user_data);
}

void *argparse_malloc(size_t size, const argparse_allocator *alloc)
{
    const argparse_allocator *a = argparse_resolve_allocator(alloc);
    if (!a || !a->alloc) {
        return NULL;
    }
    return a->alloc(size, a->user_data);
}

void *argparse_calloc(size_t count, size_t size, const argparse_allocator *alloc)
{
    size_t total = count * size;
    if (count != 0 && total / count != size) {
        return NULL;
    }
    void *ptr = argparse_malloc(total, alloc);
    if (!ptr) {
        return NULL;
    }
    memset(ptr, 0, total);
    return ptr;
}

void *argparse_realloc(void *ptr, size_t old_size, size_t new_size,
                        const argparse_allocator *alloc)
{
    const argparse_allocator *a = argparse_resolve_allocator(alloc);

    if (!ptr) {
        return a->alloc(new_size, a->user_data);
    }

    if (new_size == 0) {
        a->free(ptr, a->user_data);
        return NULL;
    }

    void *new_ptr = a->alloc(new_size, a->user_data);
    if (!new_ptr) {
        return NULL;
    }

    size_t copy_size = old_size < new_size ? old_size : new_size;
    memcpy(new_ptr, ptr, copy_size);
    a->free(ptr, a->user_data);
    return new_ptr;
}

char *argparse_strdup(const char *str, const argparse_allocator *alloc)
{
    if (!str) {
        return NULL;
    }
    size_t len = strlen(str);
    return argparse_strndup(str, len, alloc);
}

char *argparse_strndup(const char *str, size_t n, const argparse_allocator *alloc)
{
    if (!str) {
        return NULL;
    }
    char *result = (char *)argparse_malloc(n + 1, alloc);
    if (!result) {
        return NULL;
    }
    memcpy(result, str, n);
    result[n] = '\0';
    return result;
}

void argparse_free_internal(void *ptr, const argparse_allocator *alloc)
{
    if (!ptr) {
        return;
    }
    const argparse_allocator *a = argparse_resolve_allocator(alloc);
    if (!a || !a->free) {
        return;
    }
    a->free(ptr, a->user_data);
}

static void free_string_array(argparse_string_array *arr, const argparse_allocator *alloc)
{
    if (!arr) return;
    for (size_t i = 0; i < arr->count; i++) {
        argparse_free_internal(arr->items[i], alloc);
    }
    argparse_free_internal(arr->items, alloc);
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
}

static void free_value(argparse_value *val, const argparse_allocator *alloc)
{
    if (!val) return;
    argparse_free_internal(val->key, alloc);
    argparse_free_internal(val->string_value, alloc);
    if (val->array_values) {
        for (size_t i = 0; i < val->array_count; i++) {
            argparse_free_internal(val->array_values[i], alloc);
        }
        argparse_free_internal(val->array_values, alloc);
    }
    memset(val, 0, sizeof(*val));
}

void argparse_free_result(struct argparse_result *result)
{
    if (!result) return;
    const argparse_allocator *alloc = result->parser ? result->parser->allocator : NULL;
    for (size_t i = 0; i < result->value_count; i++) {
        free_value(&result->values[i], alloc);
    }
    argparse_free_internal(result->values, alloc);
    argparse_free_internal(result->error_message, alloc);
    argparse_free_internal(result, alloc);
}
