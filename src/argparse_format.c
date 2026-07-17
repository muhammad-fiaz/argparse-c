#include "argparse_internal.h"
#include "argparse-c/argparse_version.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif

#define DEFAULT_WIDTH 80
#define MIN_WIDTH 40
#define MAX_WIDTH 200

static size_t get_terminal_width(void)
{
    size_t width = DEFAULT_WIDTH;
    if (width < MIN_WIDTH) width = MIN_WIDTH;
    if (width > MAX_WIDTH) width = MAX_WIDTH;
    return width;
}

static void write_indent(FILE *fp, size_t indent)
{
    for (size_t i = 0; i < indent; i++) {
        fputc(' ', fp);
    }
}

static void write_wrapped(FILE *fp, const char *text, size_t indent, size_t width)
{
    if (!text) return;

    size_t col = indent;
    const char *line_start = text;

    while (*line_start) {
        const char *word_end = line_start;
        while (*word_end && *word_end != ' ' && *word_end != '\n') {
            word_end++;
        }

        size_t word_len = (size_t)(word_end - line_start);

        if (col + word_len > width && col > indent) {
            fputc('\n', fp);
            write_indent(fp, indent);
            col = indent;
        }

        fwrite(line_start, 1, word_len, fp);
        col += word_len;

        if (*word_end == '\n') {
            fputc('\n', fp);
            write_indent(fp, indent);
            col = indent;
            line_start = word_end + 1;
        } else if (*word_end == ' ') {
            fputc(' ', fp);
            col++;
            line_start = word_end + 1;
        } else {
            line_start = word_end;
        }
    }
}

char *argparse_format_usage(const struct argparse *parser)
{
    if (!parser) return NULL;

    size_t buf_size = 1024;
    char *buf = (char *)malloc(buf_size);
    if (!buf) return NULL;

    size_t pos = 0;
    pos += snprintf(buf + pos, buf_size - pos, "usage: %s",
                    parser->prog ? parser->prog : "program");

    bool has_options = false;
    for (size_t i = 0; i < parser->option_count; i++) {
        if (parser->options[i].action != ARGPARSE_HELP) {
            has_options = true;
            break;
        }
    }

    if (has_options) {
        pos += snprintf(buf + pos, buf_size - pos, " [options]");
    }

    if (parser->command_count > 0) {
        pos += snprintf(buf + pos, buf_size - pos, " COMMAND");
    }

    for (size_t i = 0; i < parser->positional_count; i++) {
        const struct argparse_option *opt = &parser->positionals[i];
        if (opt->required) {
            if (opt->metavar) {
                pos += snprintf(buf + pos, buf_size - pos, " %s", opt->metavar);
            } else if (opt->dest) {
                pos += snprintf(buf + pos, buf_size - pos, " %s", opt->dest);
            } else {
                pos += snprintf(buf + pos, buf_size - pos, " ARG");
            }
        } else {
            if (opt->metavar) {
                pos += snprintf(buf + pos, buf_size - pos, " [%s]", opt->metavar);
            } else if (opt->dest) {
                pos += snprintf(buf + pos, buf_size - pos, " [%s]", opt->dest);
            } else {
                pos += snprintf(buf + pos, buf_size - pos, " [ARG]");
            }
        }
    }

    if (pos < buf_size) buf[pos] = '\0';
    return buf;
}

static void format_help(const struct argparse *parser, FILE *fp)
{
    if (!parser || !fp) return;

    size_t width = get_terminal_width();
    size_t help_indent = 25;
    size_t section_indent = 2;

    char *usage = argparse_format_usage(parser);
    if (usage) {
        fprintf(fp, "%s\n\n", usage);
        free(usage);
    }

    if (parser->description) {
        write_wrapped(fp, parser->description, section_indent, width);
        fprintf(fp, "\n\n");
    }

    bool has_positionals = false;
    for (size_t i = 0; i < parser->positional_count; i++) {
        if (parser->positionals[i].help) {
            has_positionals = true;
            break;
        }
    }

    if (has_positionals) {
        fprintf(fp, "positional arguments:\n");
        for (size_t i = 0; i < parser->positional_count; i++) {
            const struct argparse_option *opt = &parser->positionals[i];
            if (!opt->help) continue;

            write_indent(fp, section_indent);
            if (opt->dest) {
                fprintf(fp, "%-*s", (int)help_indent - (int)section_indent, opt->dest);
            } else {
                fprintf(fp, "%-*s", (int)help_indent - (int)section_indent, "ARG");
            }
            write_wrapped(fp, opt->help, help_indent, width);
            fprintf(fp, "\n");
        }
        fprintf(fp, "\n");
    }

    fprintf(fp, "options:\n");
    for (size_t i = 0; i < parser->option_count; i++) {
        const struct argparse_option *opt = &parser->options[i];
        if (!opt->help) continue;

        write_indent(fp, section_indent);

        char opt_str[256];
        size_t opos = 0;

        if (opt->short_name) {
            opt_str[opos++] = '-';
            opt_str[opos++] = opt->short_name;
        }
        if (opt->long_name) {
            if (opt->short_name) {
                opt_str[opos++] = ',';
                opt_str[opos++] = ' ';
            }
            opt_str[opos++] = '-';
            opt_str[opos++] = '-';
            {
                size_t nlen = strlen(opt->long_name);
                memcpy(opt_str + opos, opt->long_name, nlen);
                opos += nlen;
            }
        }
        if (opt->metavar) {
            opt_str[opos++] = ' ';
            {
                size_t mlen = strlen(opt->metavar);
                memcpy(opt_str + opos, opt->metavar, mlen);
                opos += mlen;
            }
        }
        opt_str[opos] = '\0';

        fprintf(fp, "%-*s", (int)help_indent - (int)section_indent, opt_str);
        write_wrapped(fp, opt->help, help_indent, width);
        fprintf(fp, "\n");
    }

    if (parser->command_count > 0) {
        fprintf(fp, "\ncommands:\n");
        for (size_t i = 0; i < parser->command_count; i++) {
            const struct argparse_command *cmd = &parser->commands[i];
            write_indent(fp, section_indent);
            fprintf(fp, "%-*s", (int)help_indent - (int)section_indent,
                    cmd->name ? cmd->name : "");
            if (cmd->help) {
                write_wrapped(fp, cmd->help, help_indent, width);
            }
            fprintf(fp, "\n");
        }
    }

    if (parser->epilog) {
        fprintf(fp, "\n");
        write_wrapped(fp, parser->epilog, section_indent, width);
        fprintf(fp, "\n");
    }
}

void argparse_print_help(const struct argparse *parser)
{
    if (!parser) return;
    format_help(parser, stdout);
}

void argparse_print_usage(const struct argparse *parser)
{
    if (!parser) return;
    char *usage = argparse_format_usage(parser);
    if (usage) {
        fprintf(stdout, "%s\n", usage);
        free(usage);
    }
}

void argparse_print_version(const struct argparse *parser)
{
    (void)parser;
    printf("%s\n", argparse_version_string());
}
