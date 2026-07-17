#include "argparse_internal.h"

#include <string.h>

struct argparse_mutex *argparse_add_mutex(struct argparse *parser,
                                           struct argparse_group *group)
{
    if (!parser) return NULL;

    const argparse_allocator *a = parser->allocator;

    if (parser->mutex_count >= parser->mutex_capacity) {
        size_t new_cap = parser->mutex_capacity == 0 ? 4 : parser->mutex_capacity * 2;
        struct argparse_mutex *new_mutexes = (struct argparse_mutex *)argparse_realloc(
            parser->mutexes,
            parser->mutex_capacity * sizeof(struct argparse_mutex),
            new_cap * sizeof(struct argparse_mutex), a);
        if (!new_mutexes) return NULL;
        parser->mutexes = new_mutexes;
        parser->mutex_capacity = new_cap;
    }

    struct argparse_mutex *mutex = &parser->mutexes[parser->mutex_count];
    memset(mutex, 0, sizeof(*mutex));

    mutex->display_group = group;
    mutex->option_capacity = 4;
    mutex->options = (struct argparse_option **)argparse_calloc(
        mutex->option_capacity, sizeof(struct argparse_option *), a);

    if (!mutex->options) return NULL;

    parser->mutex_count++;
    return mutex;
}

void argparse_mutex_add_option(struct argparse_mutex *mutex,
                                struct argparse_option *option)
{
    if (!mutex || !option) return;

    if (mutex->option_count >= mutex->option_capacity) {
        size_t new_cap = mutex->option_capacity * 2;
        struct argparse_option **new_opts = (struct argparse_option **)argparse_realloc(
            mutex->options,
            mutex->option_capacity * sizeof(struct argparse_option *),
            new_cap * sizeof(struct argparse_option *), NULL);
        if (!new_opts) return;
        mutex->options = new_opts;
        mutex->option_capacity = new_cap;
    }

    mutex->options[mutex->option_count] = option;
    mutex->option_count++;
}

int argparse_mutex_check(struct argparse_mutex *mutex, struct argparse_result *result,
                          const char **conflict_name)
{
    if (!mutex) return 0;

    int set_count = 0;
    const char *first_name = NULL;

    for (size_t i = 0; i < mutex->option_count; i++) {
        struct argparse_option *opt = mutex->options[i];
        if (!opt || !opt->dest) continue;

        bool is_set = false;
        if (result) {
            argparse_value *val = argparse_result_find(result, opt->dest);
            if (val && val->is_set) {
                is_set = true;
            }
        }

        if (is_set) {
            set_count++;
            if (set_count == 1) {
                first_name = opt->long_name ? opt->long_name : "";
            }
        }
    }

    if (set_count > 1) {
        if (conflict_name) *conflict_name = first_name;
        return -1;
    }
    return 0;
}
