#include "argparse_internal.h"

#include <string.h>

struct argparse_group *argparse_add_group(struct argparse *parser,
                                           const char *title,
                                           const char *description)
{
    if (!parser) return NULL;

    const argparse_allocator *a = parser->allocator;

    if (parser->group_count >= parser->group_capacity) {
        size_t new_cap = parser->group_capacity == 0 ? 4 : parser->group_capacity * 2;
        struct argparse_group *new_groups = (struct argparse_group *)argparse_realloc(
            parser->groups,
            parser->group_capacity * sizeof(struct argparse_group),
            new_cap * sizeof(struct argparse_group), a);
        if (!new_groups) return NULL;
        parser->groups = new_groups;
        parser->group_capacity = new_cap;
    }

    struct argparse_group *group = &parser->groups[parser->group_count];
    memset(group, 0, sizeof(*group));

    group->title = argparse_strdup(title, a);
    group->description = argparse_strdup(description, a);
    group->option_capacity = 4;
    group->options = (struct argparse_option **)argparse_calloc(
        group->option_capacity, sizeof(struct argparse_option *), a);

    if (!group->title || !group->options) {
        argparse_free_internal(group->title, a);
        argparse_free_internal(group->description, a);
        argparse_free_internal(group->options, a);
        return NULL;
    }

    parser->group_count++;
    return group;
}

void argparse_group_add_option(struct argparse_group *group,
                                struct argparse_option *option)
{
    if (!group || !option) return;

    if (group->option_count >= group->option_capacity) {
        size_t new_cap = group->option_capacity * 2;
        struct argparse_option **new_opts = (struct argparse_option **)argparse_realloc(
            group->options,
            group->option_capacity * sizeof(struct argparse_option *),
            new_cap * sizeof(struct argparse_option *), NULL);
        if (!new_opts) return;
        group->options = new_opts;
        group->option_capacity = new_cap;
    }

    group->options[group->option_count] = option;
    group->option_count++;
    option->group = group;
}
