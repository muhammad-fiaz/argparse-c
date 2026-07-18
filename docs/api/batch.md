# Batch Registration API Reference

Register multiple options at once from an array of definitions.

## Functions

### `argparse_add_options_batch`

```c
int argparse_add_options_batch(struct argparse *parser,
                                const argparse_option_def *defs,
                                size_t count);
```

Register multiple options at once. Returns 0 on success, -1 on failure.

## Structures

### `argparse_option_def`

```c
typedef struct argparse_option_def {
    char short_name;          // Short flag character, or 0
    const char *long_name;    // Long name without "--" prefix
    const char *dest;         // Destination key in result namespace
    const char *help;         // Help text
    const char *metavar;      // Metavar for help output
    const char *default_value; // Default value string
    argparse_nargs nargs;     // Number of arguments
    argparse_type type;       // Value type
    argparse_action action;   // Action to perform
    bool required;            // Whether option is required
} argparse_option_def;
```
