#include "argparse_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif

static char *trim_whitespace(char *str)
{
    if (!str) return NULL;
    while (*str && isspace((unsigned char)*str)) str++;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) *end-- = '\0';
    return str;
}

void argparse_set_env_prefix(struct argparse *parser, const char *prefix)
{
    if (!parser) return;
    const argparse_allocator *a = parser->allocator;
    argparse_free_internal(parser->env_prefix, a);
    parser->env_prefix = argparse_strdup(prefix ? prefix : "", a);
}

void argparse_option_set_env(struct argparse_option *option, const char *env_var)
{
    if (!option) return;
    argparse_free_internal(option->env_var, NULL);
    option->env_var = argparse_strdup(env_var, NULL);
}

int argparse_load_env(struct argparse *parser, struct argparse_result *result)
{
    if (!parser || !result) return -1;
    if (!parser->env_prefix || strlen(parser->env_prefix) == 0) return 0;

    const argparse_allocator *a = parser->allocator;

    for (size_t i = 0; i < parser->option_count; i++) {
        struct argparse_option *opt = &parser->options[i];
        if (!opt->dest) continue;

        char env_name[256];
        snprintf(env_name, sizeof(env_name), "%s_%s", parser->env_prefix, opt->dest);
        for (char *p = env_name; *p; p++) {
            if (*p == '-') *p = '_';
            *p = (char)toupper((unsigned char)*p);
        }

        const char *env_val = getenv(env_name);
        if (env_val) {
            argparse_free_internal(opt->default_value, a);
            opt->default_value = argparse_strdup(env_val, a);
        }
    }

    return 0;
}

argparse_error_code argparse_load_config(struct argparse *parser, const char *filename)
{
    if (!parser || !filename) return ARGPARSE_ERROR_INVALID_CONFIG;

    const argparse_allocator *a = parser->allocator;
    FILE *fp = fopen(filename, "r");
    if (!fp) return ARGPARSE_ERROR_INVALID_CONFIG;

    char line[4096];

    while (fgets(line, sizeof(line), fp)) {
        char *trimmed = trim_whitespace(line);
        if (*trimmed == '\0' || *trimmed == '#' || *trimmed == '[') continue;

        char *eq = strchr(trimmed, '=');
        if (!eq) continue;

        *eq = '\0';
        char *key = trim_whitespace(trimmed);
        char *value = trim_whitespace(eq + 1);

        if ((*value == '"' && *(value + strlen(value) - 1) == '"') ||
            (*value == '\'' && *(value + strlen(value) - 1) == '\'')) {
            value++;
            value[strlen(value) - 1] = '\0';
        }

        for (size_t i = 0; i < parser->option_count; i++) {
            struct argparse_option *opt = &parser->options[i];
            if ((opt->dest && argparse_streq(opt->dest, key)) ||
                (opt->long_name && argparse_streq(opt->long_name, key))) {
                argparse_free_internal(opt->default_value, a);
                opt->default_value = argparse_strdup(value, a);
                break;
            }
        }
    }

    fclose(fp);
    return ARGPARSE_OK;
}

int argparse_load_response_file(struct argparse *parser, const char *filename,
                                 int *out_argc, const char ***out_argv)
{
    if (!parser || !filename || !out_argc || !out_argv) return -1;

    FILE *fp = fopen(filename, "r");
    if (!fp) return -1;

    size_t capacity = 16;
    size_t count = 0;
    char **argv = (char **)malloc(capacity * sizeof(char *));
    if (!argv) { fclose(fp); return -1; }

    char line[4096];
    while (fgets(line, sizeof(line), fp)) {
        char *trimmed = trim_whitespace(line);
        if (*trimmed == '\0' || *trimmed == '#') continue;

        size_t len = strlen(trimmed);
        if (len > 0 && trimmed[len - 1] == '\n') trimmed[len - 1] = '\0';

        char *token = strtok(trimmed, " \t");
        while (token) {
            if (count >= capacity) {
                capacity *= 2;
                char **new_argv = (char **)realloc(argv, capacity * sizeof(char *));
                if (!new_argv) {
                    for (size_t i = 0; i < count; i++) free(argv[i]);
                    free(argv);
                    fclose(fp);
                    return -1;
                }
                argv = new_argv;
            }
            argv[count] = strdup(token);
            if (!argv[count]) {
                for (size_t i = 0; i < count; i++) free(argv[i]);
                free(argv);
                fclose(fp);
                return -1;
            }
            count++;
            token = strtok(NULL, " \t");
        }
    }

    fclose(fp);

    *out_argc = (int)count;
    *out_argv = (const char **)argv;
    return 0;
}

void argparse_free_response_file(int argc, const char **argv)
{
    if (!argv) return;
    for (int i = 0; i < argc; i++) {
        free((void *)argv[i]);
    }
    free((void *)argv);
}
