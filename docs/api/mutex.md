---
title: "Mutex API Reference"
description: "Mutually exclusive option groups prevent multiple conflicting options from being specified together. struct argparse_mutex *argparse_add_mutex(struct argparse *parser, Create a new mutually exclusive option group. At most one option in the group may be specified on the command line. void argparse_mutex_add_option(struct argparse_mutex *mutex, Add an option to a mutually exclusive group. int main(void) {"
---

# Mutex API Reference

Mutually exclusive option groups prevent multiple conflicting options from being specified together.

## Functions

### `argparse_add_mutex`

```c
struct argparse_mutex *argparse_add_mutex(struct argparse *parser,
                                          struct argparse_group *group);
```

Create a new mutually exclusive option group. At most one option in the group may be specified on the command line.

**Parameters:**
- `parser` — The parser to add the mutex group to. Must not be NULL.
- `group` — Optional display group for help output. May be NULL.

**Returns:** Pointer to the new mutex group, or NULL on allocation failure.

### `argparse_mutex_add_option`

```c
void argparse_mutex_add_option(struct argparse_mutex *mutex,
                               struct argparse_option *option);
```

Add an option to a mutually exclusive group.

**Parameters:**
- `mutex` — The mutex group. Must not be NULL.
- `option` — The option to add. Must not be NULL.

## Example

```c
#include <argparse-c/argparse.h>

int main(void) {
    struct argparse *parser = argparse_new("tool", "Example tool");

    struct argparse_group *grp = argparse_add_group(parser, "Output", "Output format");
    struct argparse_mutex *mutex = argparse_add_mutex(parser, grp);

    struct argparse_option *json_opt = argparse_add_option(
        parser, 'j', "json", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE,
        "Output as JSON", NULL);
    struct argparse_option *csv_opt = argparse_add_option(
        parser, 'c', "csv", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE,
        "Output as CSV", NULL);

    argparse_mutex_add_option(mutex, json_opt);
    argparse_mutex_add_option(mutex, csv_opt);

    const char *argv[] = {"tool", "-j", "-c"};
    struct argparse_result *result = argparse_parse(parser, 3, argv);
    /* result->error_code == ARGPARSE_ERROR_CONFLICT */

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```
