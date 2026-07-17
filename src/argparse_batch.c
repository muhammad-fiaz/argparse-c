#include "argparse_internal.h"
#include "argparse-c/argparse_batch.h"

#include <string.h>

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

static void init_option(struct argparse_option *opt)
{
    memset(opt, 0, sizeof(*opt));
    opt->nargs = ARGPARSE_NARGS_1;
    opt->type = ARGPARSE_TYPE_STRING;
    opt->action = ARGPARSE_STORE;
}

int argparse_add_options_batch(struct argparse *parser,
                                const argparse_option_def *defs,
                                size_t count)
{
    if (!parser || !defs || count == 0) return -1;

    const argparse_allocator *a = parser->allocator;

    for (size_t i = 0; i < count; i++) {
        const argparse_option_def *def = &defs[i];

        if (!def->long_name && !def->short_name) return -1;

        if (ensure_capacity((void **)&parser->options, &parser->option_count,
                            &parser->option_capacity,
                            sizeof(struct argparse_option), a) != 0) {
            return -1;
        }

        struct argparse_option *opt = &parser->options[parser->option_count];
        init_option(opt);

        opt->short_name = def->short_name;
        opt->long_name = argparse_strdup(def->long_name, a);
        opt->nargs = def->nargs;
        opt->type = def->type;
        opt->action = def->action;
        opt->required = def->required;
        opt->help = argparse_strdup(def->help, a);
        opt->dest = argparse_strdup(def->dest, a);
        opt->metavar = argparse_strdup(def->metavar, a);
        opt->default_value = argparse_strdup(def->default_value, a);

        if (!opt->dest) {
            opt->dest = auto_dest_name(def->long_name, def->short_name, a);
        }

        parser->option_count++;
    }

    return 0;
}
