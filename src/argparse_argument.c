#include "argparse_internal.h"

#include <stdlib.h>
#include <string.h>

void argparse_free(struct argparse *parser);

static void init_option(struct argparse_option *opt)
{
    memset(opt, 0, sizeof(*opt));
    opt->nargs = ARGPARSE_NARGS_1;
    opt->type = ARGPARSE_TYPE_STRING;
    opt->action = ARGPARSE_STORE;
}

struct argparse *argparse_new(const char *prog, const char *description)
{
    const argparse_allocator *a = argparse_resolve_allocator(NULL);
    struct argparse *parser = (struct argparse *)argparse_calloc(1, sizeof(*parser), a);
    if (!parser) return NULL;

    parser->prog = argparse_strdup(prog ? prog : "program", a);
    parser->description = argparse_strdup(description, a);
    parser->prefix_chars = argparse_strdup("-", a);
    parser->fromfile_prefix_chars = NULL;
    parser->env_prefix = NULL;
    parser->allocator = a;

    if (!parser->prog) {
        argparse_free(parser);
        return NULL;
    }

    return parser;
}

void argparse_free(struct argparse *parser)
{
    if (!parser) return;
    const argparse_allocator *a = parser->allocator;

    argparse_free_internal(parser->prog, a);
    argparse_free_internal(parser->description, a);
    argparse_free_internal(parser->epilog, a);
    argparse_free_internal(parser->usage, a);
    argparse_free_internal(parser->prefix_chars, a);
    argparse_free_internal(parser->fromfile_prefix_chars, a);
    argparse_free_internal(parser->env_prefix, a);

    for (size_t i = 0; i < parser->option_count; i++) {
        struct argparse_option *opt = &parser->options[i];
        argparse_free_internal(opt->long_name, a);
        argparse_free_internal(opt->help, a);
        argparse_free_internal(opt->dest, a);
        argparse_free_internal(opt->metavar, a);
        argparse_free_internal(opt->default_value, a);
        argparse_free_internal(opt->const_value, a);
        argparse_free_internal(opt->env_var, a);
        argparse_free_internal(opt->pattern, a);
        for (size_t j = 0; j < opt->choices.count; j++) {
            argparse_free_internal(opt->choices.items[j], a);
        }
        argparse_free_internal(opt->choices.items, a);
    }
    argparse_free_internal(parser->options, a);

    for (size_t i = 0; i < parser->positional_count; i++) {
        struct argparse_option *opt = &parser->positionals[i];
        argparse_free_internal(opt->long_name, a);
        argparse_free_internal(opt->help, a);
        argparse_free_internal(opt->dest, a);
        argparse_free_internal(opt->metavar, a);
        argparse_free_internal(opt->default_value, a);
        argparse_free_internal(opt->const_value, a);
        argparse_free_internal(opt->env_var, a);
        argparse_free_internal(opt->pattern, a);
        for (size_t j = 0; j < opt->choices.count; j++) {
            argparse_free_internal(opt->choices.items[j], a);
        }
        argparse_free_internal(opt->choices.items, a);
    }
    argparse_free_internal(parser->positionals, a);

    for (size_t i = 0; i < parser->group_count; i++) {
        argparse_free_internal(parser->groups[i].title, a);
        argparse_free_internal(parser->groups[i].description, a);
        argparse_free_internal(parser->groups[i].options, a);
    }
    argparse_free_internal(parser->groups, a);

    for (size_t i = 0; i < parser->mutex_count; i++) {
        argparse_free_internal(parser->mutexes[i].options, a);
    }
    argparse_free_internal(parser->mutexes, a);

    for (size_t i = 0; i < parser->command_count; i++) {
        argparse_free_internal(parser->commands[i].name, a);
        argparse_free_internal(parser->commands[i].help, a);
        argparse_free(parser->commands[i].parser);
    }
    argparse_free_internal(parser->commands, a);

    argparse_free_internal(parser, a);
}

static int ensure_capacity(void **array, size_t *count, size_t *capacity,
                           size_t item_size, const argparse_allocator *alloc)
{
    if (*count < *capacity) return 0;
    size_t new_cap = *capacity == 0 ? 8 : *capacity * 2;
    void *new_arr = argparse_realloc(*array, *capacity * item_size,
                                      new_cap * item_size, alloc);
    if (!new_arr) return -1;
    *array = new_arr;
    *capacity = new_cap;
    return 0;
}

static char *auto_dest_name(const char *long_name, char short_name,
                            const argparse_allocator *alloc)
{
    if (long_name) {
        return argparse_strdup(long_name, alloc);
    }
    if (short_name) {
        char buf[2] = { short_name, '\0' };
        return argparse_strdup(buf, alloc);
    }
    return NULL;
}

static struct argparse_option *add_option_common(struct argparse *parser,
                                                 char short_name,
                                                 const char *long_name,
                                                 argparse_nargs nargs,
                                                 argparse_type type,
                                                 const char *help,
                                                 const char *dest)
{
    if (!parser) return NULL;
    if (!long_name && !short_name) return NULL;

    const argparse_allocator *a = parser->allocator;

    if (ensure_capacity((void **)&parser->options, &parser->option_count,
                        &parser->option_capacity, sizeof(struct argparse_option), a) != 0) {
        return NULL;
    }

    struct argparse_option *opt = &parser->options[parser->option_count];
    init_option(opt);

    opt->short_name = short_name;
    opt->long_name = argparse_strdup(long_name, a);
    opt->nargs = nargs;
    opt->type = type;
    opt->help = argparse_strdup(help, a);
    opt->dest = argparse_strdup(dest, a);

    if (!opt->dest) {
        opt->dest = auto_dest_name(long_name, short_name, a);
    }

    parser->option_count++;
    return opt;
}

struct argparse_option *argparse_add_option(struct argparse *parser,
                                             char short_name,
                                             const char *long_name,
                                             argparse_nargs nargs,
                                             argparse_type type,
                                             const char *help,
                                             const char *dest)
{
    return add_option_common(parser, short_name, long_name, nargs, type, help, dest);
}

struct argparse_option *argparse_add_positional(struct argparse *parser,
                                                 argparse_nargs nargs,
                                                 argparse_type type,
                                                 const char *help,
                                                 const char *dest)
{
    if (!parser) return NULL;

    const argparse_allocator *a = parser->allocator;

    if (ensure_capacity((void **)&parser->positionals, &parser->positional_count,
                        &parser->positional_capacity, sizeof(struct argparse_option), a) != 0) {
        return NULL;
    }

    struct argparse_option *opt = &parser->positionals[parser->positional_count];
    init_option(opt);

    opt->nargs = nargs;
    opt->type = type;
    opt->help = argparse_strdup(help, a);
    opt->dest = argparse_strdup(dest, a);
    opt->required = (nargs != ARGPARSE_NARGS_QUESTION && nargs != ARGPARSE_NARGS_STAR);

    if (nargs == ARGPARSE_NARGS_STAR || nargs == ARGPARSE_NARGS_PLUS) {
        opt->action = ARGPARSE_APPEND;
    }

    parser->positional_count++;
    return opt;
}

void argparse_set_prog(struct argparse *parser, const char *prog)
{
    if (!parser) return;
    const argparse_allocator *a = parser->allocator;
    argparse_free_internal(parser->prog, a);
    parser->prog = argparse_strdup(prog ? prog : "program", a);
}

void argparse_set_description(struct argparse *parser, const char *description)
{
    if (!parser) return;
    const argparse_allocator *a = parser->allocator;
    argparse_free_internal(parser->description, a);
    parser->description = argparse_strdup(description, a);
}

void argparse_set_epilog(struct argparse *parser, const char *epilog)
{
    if (!parser) return;
    const argparse_allocator *a = parser->allocator;
    argparse_free_internal(parser->epilog, a);
    parser->epilog = argparse_strdup(epilog, a);
}

void argparse_set_usage(struct argparse *parser, const char *usage)
{
    if (!parser) return;
    const argparse_allocator *a = parser->allocator;
    argparse_free_internal(parser->usage, a);
    parser->usage = argparse_strdup(usage, a);
}

void argparse_set_prefix_chars(struct argparse *parser, const char *chars)
{
    if (!parser) return;
    const argparse_allocator *a = parser->allocator;
    argparse_free_internal(parser->prefix_chars, a);
    parser->prefix_chars = argparse_strdup(chars ? chars : "-", a);
}

void argparse_set_fromfile_prefix_chars(struct argparse *parser, const char *chars)
{
    if (!parser) return;
    const argparse_allocator *a = parser->allocator;
    argparse_free_internal(parser->fromfile_prefix_chars, a);
    parser->fromfile_prefix_chars = argparse_strdup(chars, a);
}

void argparse_option_set_default(struct argparse_option *option,
                                 const char *default_value)
{
    if (!option) return;
    argparse_free_internal(option->default_value, NULL);
    option->default_value = argparse_strdup(default_value, NULL);
}

void argparse_option_set_const(struct argparse_option *option,
                               const char *const_value)
{
    if (!option) return;
    argparse_free_internal(option->const_value, NULL);
    option->const_value = argparse_strdup(const_value, NULL);
}

void argparse_option_set_choices(struct argparse_option *option,
                                 const char **choices, size_t count)
{
    if (!option) return;
    const argparse_allocator *alloc = NULL;

    for (size_t i = 0; i < option->choices.count; i++) {
        argparse_free_internal(option->choices.items[i], alloc);
    }
    argparse_free_internal(option->choices.items, alloc);
    option->choices.items = NULL;
    option->choices.count = 0;
    option->choices.capacity = 0;

    if (!choices || count == 0) return;

    option->choices.items = (char **)argparse_calloc(count, sizeof(char *), alloc);
    if (!option->choices.items) return;

    for (size_t i = 0; i < count; i++) {
        option->choices.items[i] = argparse_strdup(choices[i], alloc);
    }
    option->choices.count = count;
    option->choices.capacity = count;
    option->type = ARGPARSE_TYPE_ENUM;
}

void argparse_option_set_required(struct argparse_option *option, bool required)
{
    if (!option) return;
    option->required = required;
}

void argparse_option_set_metavar(struct argparse_option *option, const char *metavar)
{
    if (!option) return;
    argparse_free_internal(option->metavar, NULL);
    option->metavar = argparse_strdup(metavar, NULL);
}

void argparse_option_set_action(struct argparse_option *option, argparse_action action)
{
    if (!option) return;
    option->action = action;
}

void argparse_option_set_callback(struct argparse_option *option,
                                  argparse_callback_fn callback, void *data)
{
    if (!option) return;
    option->callback = callback;
    option->callback_data = data;
}

void argparse_option_set_validator(struct argparse_option *option,
                                   argparse_validate_fn validator)
{
    if (!option) return;
    option->validator = validator;
}

struct argparse_option *argparse_find_option_short(struct argparse *parser, char short_name)
{
    if (!parser) return NULL;
    for (size_t i = 0; i < parser->option_count; i++) {
        if (parser->options[i].short_name == short_name) {
            return &parser->options[i];
        }
    }
    return NULL;
}

struct argparse_option *argparse_find_option_long(struct argparse *parser,
                                                    const char *long_name)
{
    if (!parser || !long_name) return NULL;
    for (size_t i = 0; i < parser->option_count; i++) {
        if (parser->options[i].long_name &&
            argparse_streq(parser->options[i].long_name, long_name)) {
            return &parser->options[i];
        }
    }
    return NULL;
}
