#include "argparse_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct argparse *argparse_new(const char *prog, const char *description);

static const char *opt_dest_name(const struct argparse_option *opt)
{
    if (opt->dest) return opt->dest;
    if (opt->long_name) return opt->long_name;
    return "";
}

static int parse_long_option(struct argparse *parser, struct argparse_result *result,
                              int argc, const char **argv, int *index);
static int parse_short_option(struct argparse *parser, struct argparse_result *result,
                               int argc, const char **argv, int *index);
static int parse_positional(struct argparse *parser, struct argparse_result *result,
                             int argc, const char **argv, int *index, size_t *pos);
static void apply_defaults(struct argparse *parser, struct argparse_result *result);
static int check_required(struct argparse *parser, struct argparse_result *result);
static int check_mutex_conflicts(struct argparse *parser, struct argparse_result *result);

struct argparse_result *argparse_parse(struct argparse *parser, int argc,
                                        const char **argv)
{
    if (!parser) return NULL;

    struct argparse_result *result = argparse_result_new(parser);
    if (!result) return NULL;

    argparse_load_env(parser, result);

    int i = 1;
    size_t positional_index = 0;

    while (i < argc) {
        const char *arg = argv[i];

        if (argparse_streq(arg, "--")) {
            i++;
            while (i < argc) {
                if (parse_positional(parser, result, argc, argv, &i, &positional_index) != 0) {
                    return result;
                }
            }
            break;
        }

        if (parser->fromfile_prefix_chars &&
            strlen(parser->fromfile_prefix_chars) > 0 &&
            arg[0] == parser->fromfile_prefix_chars[0]) {
            int new_argc = 0;
            const char **new_argv = NULL;
            if (argparse_load_response_file(parser, arg + 1, &new_argc, &new_argv) == 0) {
                int j = 0;
                while (j < new_argc) {
                    const char *rarg = new_argv[j];

                    if (argparse_streq(rarg, "--")) {
                        j++;
                        while (j < new_argc) {
                            if (parse_positional(parser, result, new_argc, new_argv, &j, &positional_index) != 0) {
                                argparse_free_response_file(new_argc, new_argv);
                                return result;
                            }
                        }
                        break;
                    }

                    if (rarg[0] == '-' && rarg[1] == '-') {
                        if (parse_long_option(parser, result, new_argc, new_argv, &j) != 0) {
                            argparse_free_response_file(new_argc, new_argv);
                            return result;
                        }
                    } else if (rarg[0] == '-' && strlen(rarg) > 1) {
                        if (parse_short_option(parser, result, new_argc, new_argv, &j) != 0) {
                            argparse_free_response_file(new_argc, new_argv);
                            return result;
                        }
                    } else {
                        if (parse_positional(parser, result, new_argc, new_argv, &j, &positional_index) != 0) {
                            argparse_free_response_file(new_argc, new_argv);
                            return result;
                        }
                    }
                }
                argparse_free_response_file(new_argc, new_argv);
            }
            i++;
            continue;
        }

        if (parser->command_count > 0 && arg[0] != '-') {
            struct argparse_command *cmd = argparse_find_command(parser, arg);
            if (cmd) {
                if (!cmd->parser) {
                    cmd->parser = argparse_new(cmd->name, cmd->help);
                }
                if (cmd->parser) {
                    /* Parse remaining args through the subparser */
                    struct argparse_result *sub = argparse_parse(
                        cmd->parser, argc - i, argv + i);
                    if (sub) {
                        const argparse_allocator *alloc = parser->allocator;
                        /* Copy sub-parser values into main result */
                        for (size_t si = 0; si < sub->value_count; si++) {
                            if (!sub->values[si].key) continue;
                            argparse_value *sv = argparse_result_get_or_create(
                                result, sub->values[si].key);
                            if (sv) {
                                argparse_free_internal(sv->string_value, alloc);
                                sv->string_value = argparse_strdup(sub->values[si].string_value, alloc);
                                sv->int_value = sub->values[si].int_value;
                                sv->uint_value = sub->values[si].uint_value;
                                sv->float_value = sub->values[si].float_value;
                                sv->double_value = sub->values[si].double_value;
                                sv->bool_value = sub->values[si].bool_value;
                                sv->is_set = sub->values[si].is_set;
                            }
                            if (sub->error_code != ARGPARSE_OK) {
                                result->error_code = sub->error_code;
                                argparse_free_internal(result->error_message, alloc);
                                result->error_message = argparse_strdup(sub->error_message, alloc);
                            }
                        }
                        argparse_free_result(sub);
                    }
                }
                if (cmd->callback) {
                    cmd->callback(parser, cmd->callback_data);
                }
                return result;
            }
        }

        if (arg[0] == '-' && arg[1] == '-') {
            if (parse_long_option(parser, result, argc, argv, &i) != 0) {
                return result;
            }
            continue;
        }

        if (arg[0] == '-' && strlen(arg) > 1) {
            if (parse_short_option(parser, result, argc, argv, &i) != 0) {
                return result;
            }
            continue;
        }

        if (parse_positional(parser, result, argc, argv, &i, &positional_index) != 0) {
            return result;
        }
    }

    apply_defaults(parser, result);

    if (check_required(parser, result) != 0) {
        return result;
    }

    if (check_mutex_conflicts(parser, result) != 0) {
        return result;
    }

    result->error_code = ARGPARSE_OK;
    return result;
}

static int parse_long_option(struct argparse *parser, struct argparse_result *result,
                              int argc, const char **argv, int *index)
{
    const char *arg = argv[*index];
    const char *option_str = arg + 2;

    const char *eq = strchr(option_str, '=');
    size_t name_len = eq ? (size_t)(eq - option_str) : strlen(option_str);

    char *name = argparse_strndup(option_str, name_len, parser->allocator);
    if (!name) {
        argparse_set_error(result, ARGPARSE_ERROR_OUT_OF_MEMORY, NULL, parser->allocator);
        return -1;
    }

    struct argparse_option *opt = NULL;
    int ambiguous = 0;

    for (size_t i = 0; i < parser->option_count; i++) {
        if (!parser->options[i].long_name) continue;

        if (argparse_starts_with(parser->options[i].long_name, name)) {
            if (argparse_streq(parser->options[i].long_name, name)) {
                opt = &parser->options[i];
                ambiguous = 0;
                break;
            } else if (!opt) {
                opt = &parser->options[i];
                ambiguous = 1;
            } else {
                ambiguous = 1;
            }
        }
    }

    argparse_free_internal(name, parser->allocator);

    if (!opt) {
        char msg[256];
        int best_distance = -1;
        const char *best_match = NULL;

        for (size_t i = 0; i < parser->option_count; i++) {
            if (!parser->options[i].long_name) continue;
            int dist = argparse_levenshtein_distance(option_str,
                                                       parser->options[i].long_name);
            if (dist >= 0 && (best_distance < 0 || dist < best_distance)) {
                best_distance = dist;
                best_match = parser->options[i].long_name;
            }
        }

        if (best_match && best_distance <= 3) {
            snprintf(msg, sizeof(msg), "unknown option '%s', did you mean '%s'?",
                     option_str, best_match);
        } else {
            snprintf(msg, sizeof(msg), "unknown option '%s'", option_str);
        }
        argparse_set_error(result, ARGPARSE_ERROR_UNKNOWN, msg, parser->allocator);
        return -1;
    }

    if (ambiguous) {
        argparse_set_error(result, ARGPARSE_ERROR_AMBIGUOUS_OPTION,
                            "option is ambiguous", parser->allocator);
        return -1;
    }

    const char *value = eq ? eq + 1 : NULL;

    if (opt->nargs == ARGPARSE_NARGS_0 || opt->action == ARGPARSE_STORE_TRUE ||
        opt->action == ARGPARSE_STORE_FALSE || opt->action == ARGPARSE_COUNT ||
        opt->action == ARGPARSE_HELP || opt->action == ARGPARSE_VERSION) {
        int ret = argparse_action_perform(opt, value, result);
        if (ret == 1 || ret == 2) {
            (*index)++;
            return 0;
        }
        if (ret != 0) {
            return -1;
        }
        (*index)++;
    } else {
        if (!value) {
            (*index)++;
            if (*index >= argc) {
                argparse_set_error(result, ARGPARSE_ERROR_MISSING_ARGUMENT,
                                    "option requires an argument", parser->allocator);
                return -1;
            }
            value = argv[*index];
        }
        int ret = argparse_action_perform(opt, value, result);
        if (ret != 0) {
            return -1;
        }
        (*index)++;
    }

    return 0;
}

static int parse_short_option(struct argparse *parser, struct argparse_result *result,
                               int argc, const char **argv, int *index)
{
    const char *arg = argv[*index];
    const char *p = arg + 1;

    while (*p) {
        char short_name = *p;
        p++;

        struct argparse_option *opt = argparse_find_option_short(parser, short_name);
        if (!opt) {
            char msg[128];
            snprintf(msg, sizeof(msg), "unknown option '-%c'", short_name);
            argparse_set_error(result, ARGPARSE_ERROR_UNKNOWN, msg, parser->allocator);
            return -1;
        }

        if (opt->nargs == ARGPARSE_NARGS_0 || opt->action == ARGPARSE_STORE_TRUE ||
            opt->action == ARGPARSE_STORE_FALSE || opt->action == ARGPARSE_COUNT ||
            opt->action == ARGPARSE_HELP || opt->action == ARGPARSE_VERSION) {
            int ret = argparse_action_perform(opt, NULL, result);
            if (ret == 1 || ret == 2) {
                (*index)++;
                return 0;
            }
            if (ret != 0) {
                return -1;
            }
        } else {
            if (*p) {
                int ret = argparse_action_perform(opt, p, result);
                if (ret != 0) {
                    return -1;
                }
                (*index)++;
                return 0;
            } else {
                (*index)++;
                if (*index >= argc) {
                    argparse_set_error(result, ARGPARSE_ERROR_MISSING_ARGUMENT,
                                        "option requires an argument", parser->allocator);
                    return -1;
                }
                int ret = argparse_action_perform(opt, argv[*index], result);
                if (ret != 0) {
                    return -1;
                }
                (*index)++;
                return 0;
            }
        }
    }

    (*index)++;
    return 0;
}

static int parse_positional(struct argparse *parser, struct argparse_result *result,
                             int argc, const char **argv, int *index, size_t *pos)
{
    if (*pos >= parser->positional_count) {
        argparse_set_error(result, ARGPARSE_ERROR_UNEXPECTED_ARGUMENT,
                            "unexpected positional argument", parser->allocator);
        return -1;
    }

    struct argparse_option *opt = &parser->positionals[*pos];
    const char *value = argv[*index];

    switch (opt->nargs) {
    case ARGPARSE_NARGS_STAR:
    case ARGPARSE_NARGS_PLUS:
        /* For greedy positionals, collect all remaining non-option args */
        while (*index < argc) {
            const char *a = argv[*index];
            if (a[0] == '-' && strlen(a) > 1) {
                /* Stop at next option */
                break;
            }
            argparse_action_perform(opt, argv[*index], result);
            (*index)++;
        }
        if (opt->nargs == ARGPARSE_NARGS_PLUS) {
            /* Check we got at least one */
            argparse_value *val = argparse_result_find(result, opt_dest_name(opt));
            if (!val || val->array_count == 0) {
                argparse_set_error(result, ARGPARSE_ERROR_MISSING_ARGUMENT,
                                    "at least one argument required", parser->allocator);
                return -1;
            }
        }
        (*pos)++;
        return 0;

    case ARGPARSE_NARGS_QUESTION:
        if (*index < argc) {
            const char *a = argv[*index];
            if (a[0] == '-' && strlen(a) > 1) {
                /* Next arg is an option, don't consume */
            } else {
                argparse_action_perform(opt, argv[*index], result);
                (*index)++;
            }
        }
        (*pos)++;
        return 0;

    case ARGPARSE_NARGS_0:
        /* No argument consumed */
        (*pos)++;
        return 0;

    default:
        /* NARGS_1, NARGS_2, NARGS_3, etc. */
        argparse_action_perform(opt, value, result);
        if (opt->nargs > 0) {
            (*pos)++;
        }
        (*index)++;
        return 0;
    }
}

static void apply_defaults(struct argparse *parser, struct argparse_result *result)
{
    for (size_t i = 0; i < parser->option_count; i++) {
        struct argparse_option *opt = &parser->options[i];
        if (!opt->dest) continue;

        argparse_value *val = argparse_result_find(result, opt->dest);
        if (!val || !val->is_set) {
            const char *default_val = opt->default_value;
            if (opt->lazy_default_fn) {
                default_val = opt->lazy_default_fn(opt->lazy_default_data);
            }
            if (default_val) {
                argparse_action_perform(opt, default_val, result);
            }
        }
    }

    for (size_t i = 0; i < parser->positional_count; i++) {
        struct argparse_option *opt = &parser->positionals[i];
        if (!opt->dest) continue;

        argparse_value *val = argparse_result_find(result, opt->dest);
        if (!val || !val->is_set) {
            const char *default_val = opt->default_value;
            if (opt->lazy_default_fn) {
                default_val = opt->lazy_default_fn(opt->lazy_default_data);
            }
            if (default_val) {
                argparse_action_perform(opt, default_val, result);
            }
        }
    }
}

static int check_required(struct argparse *parser, struct argparse_result *result)
{
    for (size_t i = 0; i < parser->option_count; i++) {
        struct argparse_option *opt = &parser->options[i];
        if (!opt->required || !opt->dest) continue;

        argparse_value *val = argparse_result_find(result, opt->dest);
        if (!val || !val->is_set) {
            char msg[256];
            snprintf(msg, sizeof(msg), "option '%s' is required",
                     opt->long_name ? opt->long_name : "");
            argparse_set_error(result, ARGPARSE_ERROR_MISSING_ARGUMENT, msg,
                                parser->allocator);
            return -1;
        }
    }

    for (size_t i = 0; i < parser->positional_count; i++) {
        struct argparse_option *opt = &parser->positionals[i];
        if (!opt->required || !opt->dest) continue;

        argparse_value *val = argparse_result_find(result, opt->dest);
        if (!val || !val->is_set) {
            char msg[256];
            snprintf(msg, sizeof(msg), "positional argument '%s' is required",
                     opt->dest ? opt->dest : "");
            argparse_set_error(result, ARGPARSE_ERROR_MISSING_ARGUMENT, msg,
                                parser->allocator);
            return -1;
        }
    }

    return 0;
}

static int check_mutex_conflicts(struct argparse *parser, struct argparse_result *result)
{
    for (size_t i = 0; i < parser->mutex_count; i++) {
        struct argparse_mutex *mutex = &parser->mutexes[i];
        const char *conflict_name = NULL;

        if (argparse_mutex_check(mutex, result, &conflict_name) != 0) {
            argparse_set_error(result, ARGPARSE_ERROR_CONFLICT,
                                "conflicting options in mutually exclusive group",
                                parser->allocator);
            return -1;
        }
    }

    return 0;
}
