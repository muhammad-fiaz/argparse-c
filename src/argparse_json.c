#include "argparse_internal.h"
#include "argparse-c/argparse_json.h"

#include <stdio.h>
#include <string.h>

static void json_write_string(FILE *fp, const char *str)
{
    if (!str) {
        fputs("null", fp);
        return;
    }
    fputc('"', fp);
    for (const char *p = str; *p; p++) {
        switch (*p) {
        case '"':  fputs("\\\"", fp); break;
        case '\\': fputs("\\\\", fp); break;
        case '\n': fputs("\\n", fp);  break;
        case '\r': fputs("\\r", fp);  break;
        case '\t': fputs("\\t", fp);  break;
        default:   fputc(*p, fp);     break;
        }
    }
    fputc('"', fp);
}

static const char *type_name_str(argparse_type type)
{
    switch (type) {
    case ARGPARSE_TYPE_STRING:  return "string";
    case ARGPARSE_TYPE_INT:     return "int";
    case ARGPARSE_TYPE_UINT:    return "uint";
    case ARGPARSE_TYPE_FLOAT:   return "float";
    case ARGPARSE_TYPE_DOUBLE:  return "double";
    case ARGPARSE_TYPE_BOOL:    return "bool";
    case ARGPARSE_TYPE_ENUM:    return "enum";
    case ARGPARSE_TYPE_NONE:    return "none";
    default:                    return "string";
    }
}

static void json_write_option(FILE *fp, const struct argparse_option *opt,
                              int is_last)
{
    fputs("    {\n", fp);

    fprintf(fp, "      \"name\": ");
    json_write_string(fp, opt->long_name);
    fprintf(fp, ",\n");

    if (opt->short_name) {
        char buf[2] = { opt->short_name, '\0' };
        fprintf(fp, "      \"short\": ");
        json_write_string(fp, buf);
        fprintf(fp, ",\n");
    } else {
        fprintf(fp, "      \"short\": null,\n");
    }

    fprintf(fp, "      \"type\": ");
    json_write_string(fp, type_name_str(opt->type));
    fprintf(fp, ",\n");

    fprintf(fp, "      \"nargs\": %d,\n", (int)opt->nargs);

    fprintf(fp, "      \"help\": ");
    json_write_string(fp, opt->help);
    fprintf(fp, ",\n");

    fprintf(fp, "      \"required\": %s,\n", opt->required ? "true" : "false");

    fprintf(fp, "      \"default\": ");
    if (opt->default_value) {
        json_write_string(fp, opt->default_value);
    } else {
        fputs("null", fp);
    }
    fprintf(fp, ",\n");

    fprintf(fp, "      \"dest\": ");
    json_write_string(fp, opt->dest);
    fprintf(fp, ",\n");

    fprintf(fp, "      \"metavar\": ");
    json_write_string(fp, opt->metavar);
    fprintf(fp, ",\n");

    fprintf(fp, "      \"choices\": [");
    for (size_t c = 0; c < opt->choices.count; c++) {
        if (c > 0) fprintf(fp, ", ");
        json_write_string(fp, opt->choices.items[c]);
    }
    fprintf(fp, "]");

    fprintf(fp, "\n    }%s\n", is_last ? "" : ",");
}

int argparse_export_json_schema(const struct argparse *parser, FILE *fp)
{
    if (!parser || !fp)
        return -1;

    fprintf(fp, "{\n");
    fprintf(fp, "  \"program\": ");
    json_write_string(fp, parser->prog);
    fprintf(fp, ",\n");
    fprintf(fp, "  \"description\": ");
    json_write_string(fp, parser->description);
    fprintf(fp, ",\n");
    fprintf(fp, "  \"epilog\": ");
    json_write_string(fp, parser->epilog);
    fprintf(fp, ",\n");

    fprintf(fp, "  \"options\": [\n");
    for (size_t i = 0; i < parser->option_count; i++) {
        json_write_option(fp, &parser->options[i],
                          i == parser->option_count - 1);
    }
    fprintf(fp, "  ],\n");

    fprintf(fp, "  \"positionals\": [\n");
    for (size_t i = 0; i < parser->positional_count; i++) {
        json_write_option(fp, &parser->positionals[i],
                          i == parser->positional_count - 1);
    }
    fprintf(fp, "  ],\n");

    fprintf(fp, "  \"groups\": [\n");
    for (size_t i = 0; i < parser->group_count; i++) {
        fprintf(fp, "    {\n");
        fprintf(fp, "      \"title\": ");
        json_write_string(fp, parser->groups[i].title);
        fprintf(fp, ",\n");
        fprintf(fp, "      \"description\": ");
        json_write_string(fp, parser->groups[i].description);
        fprintf(fp, "\n    }%s\n",
                i == parser->group_count - 1 ? "" : ",");
    }
    fprintf(fp, "  ],\n");

    fprintf(fp, "  \"commands\": [\n");
    for (size_t i = 0; i < parser->command_count; i++) {
        fprintf(fp, "    {\n");
        fprintf(fp, "      \"name\": ");
        json_write_string(fp, parser->commands[i].name);
        fprintf(fp, ",\n");
        fprintf(fp, "      \"help\": ");
        json_write_string(fp, parser->commands[i].help);
        fprintf(fp, "\n    }%s\n",
                i == parser->command_count - 1 ? "" : ",");
    }
    fprintf(fp, "  ]\n");

    fprintf(fp, "}\n");
    return 0;
}

int argparse_export_json_schema_file(const struct argparse *parser,
                                     const char *filename)
{
    if (!parser || !filename)
        return -1;

    FILE *fp = fopen(filename, "w");
    if (!fp)
        return -1;

    int result = argparse_export_json_schema(parser, fp);
    fclose(fp);
    return result;
}

static void json_write_value(FILE *fp, const argparse_value *val)
{
    if (!val->is_set) {
        fputs("null", fp);
        return;
    }

    if (val->array_count > 0) {
        fprintf(fp, "[");
        for (size_t i = 0; i < val->array_count; i++) {
            if (i > 0) fprintf(fp, ", ");
            if (val->array_values[i]) {
                json_write_string(fp, val->array_values[i]);
            } else {
                fputs("null", fp);
            }
        }
        fprintf(fp, "]");
        return;
    }

    if (val->string_value) {
        json_write_string(fp, val->string_value);
    } else if (val->bool_value) {
        fputs("true", fp);
    } else if (val->int_value != 0) {
        fprintf(fp, "%d", val->int_value);
    } else if (val->uint_value != 0) {
        fprintf(fp, "%u", val->uint_value);
    } else if (val->float_value != 0.0f) {
        fprintf(fp, "%g", (double)val->float_value);
    } else if (val->double_value != 0.0) {
        fprintf(fp, "%g", val->double_value);
    } else {
        fputs("null", fp);
    }
}

int argparse_export_json_values(const struct argparse_result *result,
                                FILE *fp)
{
    if (!result || !fp)
        return -1;

    fprintf(fp, "{\n");
    for (size_t i = 0; i < result->value_count; i++) {
        fprintf(fp, "  ");
        json_write_string(fp, result->values[i].key);
        fprintf(fp, ": ");
        json_write_value(fp, &result->values[i]);
        fprintf(fp, "%s\n",
                i == result->value_count - 1 ? "" : ",");
    }
    fprintf(fp, "}\n");
    return 0;
}

int argparse_export_json_values_file(const struct argparse_result *result,
                                     const char *filename)
{
    if (!result || !filename)
        return -1;

    FILE *fp = fopen(filename, "w");
    if (!fp)
        return -1;

    int result_code = argparse_export_json_values(result, fp);
    fclose(fp);
    return result_code;
}
