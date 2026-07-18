---
title: "Groups API"
description: "Organize options into logical groups for help output."
---

# Groups API

Organizing options into logical groups for help output.

## Functions

### `argparse_add_group`

```c
struct argparse_group *argparse_add_group(struct argparse *parser,
                                          const char *title,
                                          const char *description);
```

Create a new argument group.

**Parameters:**
- `title` — Group title shown in help output
- `description` — Optional description below the title

**Returns:** Group pointer, or NULL on failure.

### `argparse_group_add_option`

```c
void argparse_group_add_option(struct argparse_group *group,
                               struct argparse_option *option);
```

Add an option to a group.

## Example

```c
struct argparse *parser = argparse_new("myapp", "Application");

/* Create a group */
struct argparse_group *network = argparse_add_group(parser,
    "Network Options", "Configure network settings");

/* Add options to the group */
struct argparse_option *host_opt = argparse_add_option(parser,
    'H', "host", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
    "Server host", "host");
argparse_group_add_option(network, host_opt);

struct argparse_option *port_opt = argparse_add_option(parser,
    'p', "port", ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT,
    "Server port", "port");
argparse_group_add_option(network, port_opt);
```

Help output:

```
usage: myapp [-h] [-H HOST] [-p PORT]

Application

Network Options:
  Configure network settings
  -H, --host HOST  Server host
  -p, --port PORT  Server port
```

## Notes

- Options not added to any group appear in the default "options" group
- Groups are displayed in the order they are created
- Groups can be used with mutually exclusive groups
