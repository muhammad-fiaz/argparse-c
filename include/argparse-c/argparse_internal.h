#ifndef ARGPARSE_INTERNAL_H
#define ARGPARSE_INTERNAL_H

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif

#include "argparse_types.h"
#include "argparse_memory.h"
#include "argparse_validator.h"

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct argparse_string_array {
    char **items;
    size_t count;
    size_t capacity;
} argparse_string_array;

struct argparse_option {
    char short_name;
    char *long_name;
    argparse_nargs nargs;
    argparse_type type;
    argparse_action action;
    char *help;
    char *dest;
    char *default_value;
    char *const_value;
    char *metavar;
    bool required;
    argparse_string_array choices;
    argparse_callback_fn callback;
    void *callback_data;
    argparse_validate_fn validator;
    char *env_var;
    struct argparse_group *group;

    int range_min_int;
    int range_max_int;
    float range_min_float;
    float range_max_float;
    char *pattern;

    argparse_lazy_default_fn lazy_default_fn;
    void *lazy_default_data;
};

struct argparse {
    char *prog;
    char *description;
    char *epilog;
    char *usage;
    char *prefix_chars;
    char *fromfile_prefix_chars;
    char *env_prefix;

    struct argparse_option *options;
    size_t option_count;
    size_t option_capacity;

    struct argparse_option *positionals;
    size_t positional_count;
    size_t positional_capacity;

    struct argparse_group *groups;
    size_t group_count;
    size_t group_capacity;

    struct argparse_mutex *mutexes;
    size_t mutex_count;
    size_t mutex_capacity;

    struct argparse_command *commands;
    size_t command_count;
    size_t command_capacity;

    const argparse_allocator *allocator;

    bool frozen;
};

typedef struct argparse_value {
    char *key;
    char *string_value;
    int int_value;
    unsigned int uint_value;
    float float_value;
    double double_value;
    bool bool_value;
    bool is_set;
    char **array_values;
    size_t array_count;
    size_t array_capacity;
} argparse_value;

struct argparse_result {
    argparse_value *values;
    size_t value_count;
    size_t value_capacity;
    argparse_error_code error_code;
    char *error_message;
    struct argparse *parser;
};

struct argparse_group {
    char *title;
    char *description;
    struct argparse_option **options;
    size_t option_count;
    size_t option_capacity;
};

struct argparse_mutex {
    struct argparse_group *display_group;
    struct argparse_option **options;
    size_t option_count;
    size_t option_capacity;
};

struct argparse_command {
    char *name;
    char *help;
    argparse_callback_fn callback;
    void *callback_data;
    struct argparse *parser;
};

bool argparse_starts_with(const char *str, const char *prefix);
bool argparse_streq(const char *a, const char *b);
int argparse_levenshtein_distance(const char *s1, const char *s2);

void *argparse_malloc(size_t size, const argparse_allocator *alloc);
void *argparse_calloc(size_t count, size_t size, const argparse_allocator *alloc);
void *argparse_realloc(void *ptr, size_t old_size, size_t new_size,
                        const argparse_allocator *alloc);
char *argparse_strdup(const char *str, const argparse_allocator *alloc);
char *argparse_strndup(const char *str, size_t n, const argparse_allocator *alloc);
void argparse_free_internal(void *ptr, const argparse_allocator *alloc);
const argparse_allocator *argparse_resolve_allocator(const argparse_allocator *alloc);

const char *argparse_error_string(argparse_error_code code);
void argparse_set_error(struct argparse_result *result, argparse_error_code code,
                         const char *message, const argparse_allocator *alloc);

struct argparse_result *argparse_result_new(struct argparse *parser);
argparse_value *argparse_result_find(struct argparse_result *result, const char *key);
argparse_value *argparse_result_get_or_create(struct argparse_result *result, const char *key);
void argparse_free_result(struct argparse_result *result);

struct argparse_option *argparse_find_option_short(struct argparse *parser, char short_name);
struct argparse_option *argparse_find_option_long(struct argparse *parser, const char *long_name);

int argparse_action_perform(struct argparse_option *opt, const char *value,
                             struct argparse_result *result);

int argparse_load_env(struct argparse *parser, struct argparse_result *result);
int argparse_load_response_file(struct argparse *parser, const char *filename,
                                 int *out_argc, const char ***out_argv);
void argparse_free_response_file(int argc, const char **argv);

struct argparse_command *argparse_find_command(struct argparse *parser, const char *name);

int argparse_mutex_check(struct argparse_mutex *mutex, struct argparse_result *result,
                          const char **conflict_name);

void argparse_set_validator_context(struct argparse_option *opt);
struct argparse_option *argparse_get_validator_context(void);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_INTERNAL_H */
