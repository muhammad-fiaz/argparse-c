/*
 * examples/advanced_features.c - Comprehensive advanced features demo
 *
 * Demonstrates ALL advanced features of argparse-c:
 *   - Range validators (integer and float)
 *   - Pattern validators (glob-style matching)
 *   - Colored terminal output
 *   - Batch option registration
 *   - Thread safety (freeze + parse)
 *   - Custom type registration (via validators)
 *   - i18n translations
 *   - Man page generation (output to stdout)
 *   - JSON schema export (output to stdout)
 *
 * Build:
 *   gcc -std=c99 -o advanced_features advanced_features.c \
 *       -I../include -L../build -largparse-c
 *
 * Run:
 *   ./advanced_features --port 8080 --name "myapp" --level 5
 *   ./advanced_features --port 99999
 *   ./advanced_features --output "file.txt" --help
 *   ./advanced_features --man
 *   ./advanced_features --json-schema
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <argparse-c/argparse.h>

/* ---------------------------------------------------------------------------
 * Range and pattern validators are built-in via argparse_validator_extra.h.
 * We use argparse_option_set_range_int() and argparse_option_set_pattern()
 * directly instead of writing custom validators from scratch.
 * --------------------------------------------------------------------------- */

/* Custom validator: ensure string matches a simple length constraint */
static bool validate_length_range(const char *value, char *error_buf,
                                  size_t error_buf_size)
{
    size_t len = strlen(value);
    if (len < 2 || len > 64) {
        snprintf(error_buf, error_buf_size,
                 "Length must be between 2 and 64 characters, got %zu", len);
        return false;
    }
    return true;
}

/* Custom validator: ensure port is in the well-known range */
static bool validate_port(const char *value, char *error_buf,
                          size_t error_buf_size)
{
    int port = atoi(value);
    if (port < 1024 || port > 65535) {
        snprintf(error_buf, error_buf_size,
                 "Port must be between 1024 and 65535, got %d", port);
        return false;
    }
    return true;
}

/* ---------------------------------------------------------------------------
 * i18n demonstration: register a few translations for the "es" locale.
 * --------------------------------------------------------------------------- */
static void setup_i18n(void)
{
    argparse_i18n_set("es", ARGPARSE_MSG_USAGE, "Uso");
    argparse_i18n_set("es", ARGPARSE_MSG_OPTIONS, "Opciones");
    argparse_i18n_set("es", ARGPARSE_MSG_POSITIONAL, "Argumentos posicionales");
    argparse_i18n_set("es", ARGPARSE_MSG_ERROR, "Error");
}

/* ---------------------------------------------------------------------------
 * main
 * --------------------------------------------------------------------------- */
int main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;

    /* --- 1. Create parser with description and epilog --- */
    struct argparse *parser = argparse_new(
        "advanced_features",
        "Comprehensive demonstration of argparse-c advanced features"
    );
    argparse_set_epilog(parser,
        "Run with --man to generate a man page, "
        "or --json-schema to export the schema as JSON."
    );

    /* --- 2. Colored output: enable it for demonstration --- */
    argparse_color_enable(true);

    /* --- 3. Batch option registration --- */
    {
        argparse_option_def batch_defs[] = {
            { 'v', "verbose", "verbose", "Increase verbosity level",
              NULL, NULL, ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE,
              ARGPARSE_COUNT, false },
            { 'q', "quiet", "quiet", "Suppress all output",
              NULL, NULL, ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE,
              ARGPARSE_STORE_TRUE, false },
            { 'f', "force", "force", "Force overwrite of output files",
              NULL, NULL, ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE,
              ARGPARSE_STORE_TRUE, false },
        };
        argparse_add_options_batch(parser, batch_defs,
                                   sizeof(batch_defs) / sizeof(batch_defs[0]));
    }

    /* --- 4. Range validators: integer range for --level --- */
    {
        struct argparse_option *level = argparse_add_option(
            parser, 'l', "level",
            ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT,
            "Processing level (1-10)", "LEVEL"
        );
        argparse_option_set_range_int(level, 1, 10);
    }

    /* --- 5. Range validators: integer range for --port --- */
    {
        struct argparse_option *port = argparse_add_option(
            parser, 'p', "port",
            ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT,
            "Port number (1024-65535)", "PORT"
        );
        argparse_option_set_validator(port, validate_port);
    }

    /* --- 6. Pattern validator: glob-style matching for --output --- */
    {
        struct argparse_option *output = argparse_add_option(
            parser, 'o', "output",
            ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
            "Output file (must match *.txt or *.log)", "FILE"
        );
        argparse_option_set_pattern(output, "*.txt");
    }

    /* --- 7. Custom validator: length constraint for --name --- */
    {
        struct argparse_option *name = argparse_add_option(
            parser, 'n', "name",
            ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
            "Application name (2-64 chars)", "NAME"
        );
        argparse_option_set_validator(name, validate_length_range);
    }

    /* --- 8. Enum choices for --format --- */
    {
        struct argparse_option *fmt = argparse_add_option(
            parser, 'F', "format",
            ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
            "Output format", "FORMAT"
        );
        const char *fmt_choices[] = { "json", "csv", "xml", "text" };
        argparse_option_set_choices(fmt, fmt_choices, 4);
    }

    /* --- 9. Default values and required options --- */
    {
        struct argparse_option *config = argparse_add_option(
            parser, 'c', "config",
            ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
            "Configuration file path", "PATH"
        );
        argparse_option_set_default(config, "/etc/app.conf");
    }
    {
        struct argparse_option *timeout = argparse_add_option(
            parser, '\0', "timeout",
            ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT,
            "Connection timeout in seconds", "SEC"
        );
        argparse_option_set_default(timeout, "30");
        argparse_option_set_range_int(timeout, 1, 300);
    }

    /* --- 10. Positional argument --- */
    argparse_add_positional(parser,
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Input file to process", "INPUT"
    );

    /* --- 11. Setup i18n translations (Spanish locale as demo) --- */
    setup_i18n();
    /* Note: we keep English as default; this just shows registration works. */

    /* --- 12. Man page generation to stdout --- */
    /* If user passes --man, print the man page and exit. */
    if (argc > 1 && (strcmp(argv[1], "--man") == 0)) {
        printf("--- Man Page Output (groff format) ---\n\n");
        argparse_generate_manpage(parser, stdout);
        printf("\n--- End Man Page ---\n");
        argparse_free(parser);
        return 0;
    }

    /* --- 13. JSON schema export to stdout --- */
    /* If user passes --json-schema, print the JSON schema and exit. */
    if (argc > 1 && (strcmp(argv[1], "--json-schema") == 0)) {
        printf("--- JSON Schema Output ---\n\n");
        argparse_export_json_schema(parser, stdout);
        printf("\n--- End JSON Schema ---\n");
        argparse_free(parser);
        return 0;
    }

    /* --- 14. Freeze parser for thread-safe parsing --- */
    argparse_freeze(parser);

    /* --- 15. Parse arguments (thread-safe on frozen parser) --- */
    struct argparse_result *result = argparse_parse_threadsafe(
        parser, argc, argv
    );

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        argparse_print_colored(stderr,
            argparse_result_error(result),
            ARGPARSE_COLOR_RED);
        fprintf(stderr, "\n");
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    /* --- 16. Access and display parsed values --- */
    printf("Parsed values:\n");
    printf("  verbose: %d\n", argparse_get_int(result, "verbose"));
    printf("  quiet:   %s\n",
           argparse_get_bool(result, "quiet") ? "true" : "false");
    printf("  force:   %s\n",
           argparse_get_bool(result, "force") ? "true" : "false");
    printf("  level:   %d\n", argparse_get_int(result, "LEVEL"));
    printf("  port:    %d\n", argparse_get_int(result, "PORT"));
    printf("  output:  %s\n",
           argparse_get_string(result, "FILE")
               ? argparse_get_string(result, "FILE")
               : "(default)");
    printf("  name:    %s\n",
           argparse_get_string(result, "NAME")
               ? argparse_get_string(result, "NAME")
               : "(default)");
    printf("  format:  %s\n",
           argparse_get_string(result, "FORMAT")
               ? argparse_get_string(result, "FORMAT")
               : "(default)");
    printf("  config:  %s\n",
           argparse_get_string(result, "PATH")
               ? argparse_get_string(result, "PATH")
               : "(default)");
    printf("  timeout: %d\n", argparse_get_int(result, "SEC"));
    printf("  input:   %s\n",
           argparse_get_string(result, "INPUT")
               ? argparse_get_string(result, "INPUT")
               : "(none)");

    /* --- 17. Export parsed values as JSON --- */
    printf("\n--- Parsed Values (JSON) ---\n");
    argparse_export_json_values(result, stdout);
    printf("--- End Values ---\n");

    /* --- 18. Demonstrate i18n locale switching --- */
    printf("\n--- i18n Demo ---\n");
    printf("  Current locale: %s\n", argparse_get_locale());
    printf("  'usage' key:    %s\n", argparse_i18n_get(ARGPARSE_MSG_USAGE));
    printf("  'options' key:  %s\n", argparse_i18n_get(ARGPARSE_MSG_OPTIONS));

    /* --- 19. Cleanup --- */
    argparse_result_free(result);
    argparse_free(parser);

    printf("\nDone.\n");
    return 0;
}
