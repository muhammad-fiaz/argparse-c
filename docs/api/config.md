---
title: "Config API"
description: "Configuration file and environment variable support. void argparse_set_env_prefix(struct argparse *parser, const char *prefix); Set the environment variable prefix. When set, the parser checks for environment variables named `PREFIX_OPTIONNAME` for options not on the command line. void argparse_option_set_env(struct argparse_option *option, Set a specific environment variable for an option (overrides prefix-based naming). argparse_error_code argparse_load_config(struct argparse *parser, Load option values from a configuration file. int main(int argc, const char **argv) { The environment variable name is derived by:"
---

# Config API

Configuration file and environment variable support.

## Functions

### `argparse_set_env_prefix`

```c
void argparse_set_env_prefix(struct argparse *parser, const char *prefix);
```

Set the environment variable prefix. When set, the parser checks for environment variables named `PREFIX_OPTIONNAME` for options not on the command line.

**Example:** With prefix "MYAPP" and option `--output-file`, the parser checks `MYAPP_OUTPUT_FILE`.

### `argparse_option_set_env`

```c
void argparse_option_set_env(struct argparse_option *option,
                             const char *env_var);
```

Set a specific environment variable for an option (overrides prefix-based naming).

### `argparse_load_config`

```c
argparse_error_code argparse_load_config(struct argparse *parser,
                                         const char *filename);
```

Load option values from a configuration file.

**Supported formats:**
- Lines starting with `#` are comments
- `option = value` sets option values
- Sections `[section]` are ignored
- Boolean options: `option = true` or `option = false`

## Precedence Order

1. **Command-line arguments** (highest)
2. **Environment variables**
3. **Configuration files** (lowest)

## Example

```c
#include <argparse-c/argparse.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("myapp", "Application");

    argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Output file", "output");
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose mode", "verbose");
    argparse_add_option(parser, 'n', "count", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Count", "count");

    /* Set environment prefix */
    argparse_set_env_prefix(parser, "MYAPP");

    /* Load config file */
    argparse_load_config(parser, ".myapp.conf");

    /* Parse command line (overrides env and config) */
    struct argparse_result *result = argparse_parse(parser, argc, argv);

    /* Access values */
    const char *output = argparse_get_string(result, "output");
    bool verbose = argparse_get_bool(result, "verbose");
    int count = argparse_get_int(result, "count");

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Environment Variable Naming

The environment variable name is derived by:
1. Converting the option name to uppercase
2. Replacing hyphens with underscores
3. Prepending the prefix

| Option | Prefix | Environment Variable |
|--------|--------|---------------------|
| `--output-file` | `MYAPP` | `MYAPP_OUTPUT_FILE` |
| `--verbose` | `MYAPP` | `MYAPP_VERBOSE` |
| `-o` | `MYAPP` | (uses long name if available) |

## Notes

- Config file values have lower precedence than command-line arguments
- If the config file doesn't exist, `argparse_load_config` returns an error
- Environment variables override config file values
- The env prefix is copied internally
