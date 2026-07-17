#include "argparse_internal.h"

#include <stdio.h>
#include <string.h>

static void complete_bash(struct argparse *parser, int argc, const char **argv)
{
    (void)argc;
    (void)argv;
    if (!parser) return;

    for (size_t i = 0; i < parser->command_count; i++) {
        if (parser->commands[i].name) {
            printf("%s\n", parser->commands[i].name);
        }
    }

    for (size_t i = 0; i < parser->option_count; i++) {
        const struct argparse_option *opt = &parser->options[i];
        if (opt->long_name) {
            printf("--%s\n", opt->long_name);
        }
        if (opt->short_name) {
            printf("-%c\n", opt->short_name);
        }
    }
}

static void complete_zsh(struct argparse *parser, int argc, const char **argv)
{
    (void)argc;
    (void)argv;
    if (!parser) return;

    for (size_t i = 0; i < parser->command_count; i++) {
        if (parser->commands[i].name) {
            printf("%s:%s\n", parser->commands[i].name,
                   parser->commands[i].help ? parser->commands[i].help : "");
        }
    }

    for (size_t i = 0; i < parser->option_count; i++) {
        const struct argparse_option *opt = &parser->options[i];
        if (opt->long_name) {
            printf("--%s:%s\n", opt->long_name, opt->help ? opt->help : "");
        }
        if (opt->short_name) {
            printf("-%c:%s\n", opt->short_name, opt->help ? opt->help : "");
        }
    }
}

static void complete_fish(struct argparse *parser, int argc, const char **argv)
{
    (void)argc;
    (void)argv;
    if (!parser) return;

    for (size_t i = 0; i < parser->command_count; i++) {
        if (parser->commands[i].name) {
            printf("%s\t%s\n", parser->commands[i].name,
                   parser->commands[i].help ? parser->commands[i].help : "");
        }
    }

    for (size_t i = 0; i < parser->option_count; i++) {
        const struct argparse_option *opt = &parser->options[i];
        if (opt->long_name) {
            printf("--%s\t%s\n", opt->long_name, opt->help ? opt->help : "");
        }
        if (opt->short_name) {
            printf("-%c\t%s\n", opt->short_name, opt->help ? opt->help : "");
        }
    }
}

static void complete_powershell(struct argparse *parser, int argc, const char **argv)
{
    (void)argc;
    (void)argv;
    if (!parser) return;

    for (size_t i = 0; i < parser->command_count; i++) {
        if (parser->commands[i].name) {
            printf("%s\n", parser->commands[i].name);
        }
    }

    for (size_t i = 0; i < parser->option_count; i++) {
        const struct argparse_option *opt = &parser->options[i];
        if (opt->long_name) {
            printf("--%s\n", opt->long_name);
        }
        if (opt->short_name) {
            printf("-%c\n", opt->short_name);
        }
    }
}

void argparse_complete_bash(struct argparse *parser, int argc, const char **argv)
{
    complete_bash(parser, argc, argv);
}

void argparse_complete_zsh(struct argparse *parser, int argc, const char **argv)
{
    complete_zsh(parser, argc, argv);
}

void argparse_complete_fish(struct argparse *parser, int argc, const char **argv)
{
    complete_fish(parser, argc, argv);
}

void argparse_complete_powershell(struct argparse *parser, int argc, const char **argv)
{
    complete_powershell(parser, argc, argv);
}
