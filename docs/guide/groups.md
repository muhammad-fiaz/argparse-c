---
title: Groups
description: Organizing arguments into groups
---

# Groups

Groups organize related arguments together for better help formatting and logical organization.

## Creating Groups

```c
struct argparse_group *group = argparse_group_new(
    parser,
    "Network Options",
    "Options for network configuration"
);
```

## Adding Arguments to Groups

```c
struct argparse_group *net_group = argparse_group_new(
    parser,
    "Network Options",
    "Configure network settings"
);

argparse_group_add_option(net_group, 'H', "host", ARGPARSE_NARGS_1,
                          ARGPARSE_TYPE_STRING, "Server hostname", "HOST");
argparse_group_add_option(net_group, 'p', "port", ARGPARSE_NARGS_1,
                          ARGPARSE_TYPE_INT, "Server port", "PORT");
argparse_group_add_option(net_group, 't', "timeout", ARGPARSE_NARGS_1,
                          ARGPARSE_TYPE_INT, "Connection timeout (seconds)", "SEC");
```

## Grouped Help Output

```
Usage: myapp [OPTIONS] INPUT

Network Options:
  -H, --host HOST       Server hostname
  -p, --port PORT       Server port
  -t, --timeout SEC     Connection timeout (seconds)

Application Options:
  -v, --verbose         Enable verbose output
  -o, --output FILE     Output file

General Options:
  -h, --help            Show this help message
  -V, --version         Show version
```

## Example: Multi-Group Application

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    struct argparse *parser = argparse_new("dbtool", "Database management tool");

    /* Connection group */
    struct argparse_group *conn = argparse_group_new(
        parser, "Connection", "Database connection options"
    );
    argparse_group_add_option(conn, 'H', "host", ARGPARSE_NARGS_1,
                              ARGPARSE_TYPE_STRING, "Database host", "HOST");
    argparse_group_add_option(conn, 'p', "port", ARGPARSE_NARGS_1,
                              ARGPARSE_TYPE_INT, "Database port", "PORT");
    argparse_group_add_option(conn, 'u', "user", ARGPARSE_NARGS_1,
                              ARGPARSE_TYPE_STRING, "Database user", "USER");
    argparse_group_add_option(conn, 'd', "database", ARGPARSE_NARGS_1,
                              ARGPARSE_TYPE_STRING, "Database name", "DB");

    /* Output group */
    struct argparse_group *out = argparse_group_new(
        parser, "Output", "Output formatting options"
    );
    argparse_group_add_option(out, 'f', "format", ARGPARSE_NARGS_1,
                              ARGPARSE_TYPE_STRING, "Output format (csv,json,table)", "FMT");
    argparse_group_add_option(out, 'o', "output", ARGPARSE_NARGS_1,
                              ARGPARSE_TYPE_STRING, "Output file", "FILE");

    struct argparse_result *result = argparse_parse(
        parser, argc, (const char **)argv
    );

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    if (argparse_result_should_exit(result)) {
        argparse_result_free(result);
        argparse_free(parser);
        return 0;
    }

    printf("Connecting to %s:%d as %s@%s\n",
           argparse_get_string(result, "host"),
           argparse_get_int(result, "port"),
           argparse_get_string(result, "user"),
           argparse_get_string(result, "database"));

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Next Steps

- [Subcommands](./subcommands) - Nested command structures
- [Help Formatting](./help-formatting) - Customizing help output
- [API: Groups](../api/groups) - Complete API reference
