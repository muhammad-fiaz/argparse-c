# Batch Registration

Batch registration lets you register multiple options at once from an array of definitions, reducing repetitive `argparse_add_option()` calls.

## Example

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("myapp", "Batch-registered CLI");

    // Define options in an array
    argparse_option_def defs[] = {
        { 'v', "verbose",  NULL, "Verbose output",      NULL,  NULL,
          ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, ARGPARSE_STORE, false },
        { 'o', "output",   NULL, "Output file",         "FILE", NULL,
          ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, ARGPARSE_STORE, false },
        { 'n', "count",    NULL, "Repeat count",        "NUM",  "1",
          ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, ARGPARSE_STORE, false },
        { 'd', "debug",    NULL, "Debug mode",          NULL,  NULL,
          ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, ARGPARSE_STORE, false },
        { 'h', "help",     NULL, "Show help",           NULL,  NULL,
          ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, ARGPARSE_HELP, false },
    };

    // Register all at once
    argparse_add_options_batch(parser, defs, sizeof(defs) / sizeof(defs[0]));

    struct argparse_result *result = argparse_parse(parser, argc, argv);
    // ... use results ...
    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Option Definition Structure

```c
typedef struct argparse_option_def {
    char short_name;        // Short flag character, or 0
    const char *long_name;  // Long name without "--" prefix
    const char *dest;       // Destination key in result namespace
    const char *help;       // Help text
    const char *metavar;    // Metavar for help output
    const char *default_value; // Default value string
    argparse_nargs nargs;   // Number of arguments
    argparse_type type;     // Value type
    argparse_action action; // Action to perform
    bool required;          // Whether option is required
} argparse_option_def;
```

## API

```c
int argparse_add_options_batch(struct argparse *parser,
                                const argparse_option_def *defs,
                                size_t count);
```

| Parameter | Description |
|-----------|-------------|
| `parser` | The parser to add options to |
| `defs` | Array of option definitions |
| `count` | Number of definitions in the array |

Returns `0` on success, `-1` on failure.

## Notes

- Each definition must have at least one of `short_name` or `long_name` set
- Options added via batch registration can be further configured with individual `argparse_option_set_*()` calls
- On failure, options already registered in the batch remain registered
