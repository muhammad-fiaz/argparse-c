# Options API

Creating and configuring options and positional arguments.

## Adding Options

### `argparse_add_option`

```c
struct argparse_option *argparse_add_option(
    struct argparse *parser,
    char short_name,
    const char *long_name,
    argparse_nargs nargs,
    argparse_type type,
    const char *help,
    const char *dest);
```

Add an optional argument. At least one of `short_name` or `long_name` must be provided.

**Parameters:**
- `short_name` — Single-character short option (e.g., 'v'), or 0
- `long_name` — Long option name without prefix (e.g., "verbose"), or NULL
- `nargs` — Number of arguments expected
- `type` — Value type
- `help` — Help text
- `dest` — Destination name (NULL for auto-generated)

**Returns:** Option pointer for further configuration, or NULL on failure.

### `argparse_add_positional`

```c
struct argparse_option *argparse_add_positional(
    struct argparse *parser,
    argparse_nargs nargs,
    argparse_type type,
    const char *help,
    const char *dest);
```

Add a positional argument.

## Option Configuration

### `argparse_option_set_default`

```c
void argparse_option_set_default(struct argparse_option *option,
                                 const char *default_value);
```

Set the default value.

### `argparse_option_set_const`

```c
void argparse_option_set_const(struct argparse_option *option,
                               const char *const_value);
```

Set the constant value for `STORE_CONST` actions.

### `argparse_option_set_choices`

```c
void argparse_option_set_choices(struct argparse_option *option,
                                 const char **choices,
                                 size_t count);
```

Restrict to allowed choices (sets type to ENUM).

### `argparse_option_set_required`

```c
void argparse_option_set_required(struct argparse_option *option,
                                  bool required);
```

Mark as required.

### `argparse_option_set_metavar`

```c
void argparse_option_set_metavar(struct argparse_option *option,
                                 const char *metavar);
```

Set the metavar in help output.

### `argparse_option_set_action`

```c
void argparse_option_set_action(struct argparse_option *option,
                                argparse_action action);
```

Override the default action.

### `argparse_option_set_callback`

```c
void argparse_option_set_callback(struct argparse_option *option,
                                  argparse_callback_fn callback,
                                  void *data);
```

Set a callback function.

### `argparse_option_set_validator`

```c
void argparse_option_set_validator(struct argparse_option *option,
                                   argparse_validate_fn validator);
```

Set a custom validator.

## Example

```c
struct argparse_option *opt = argparse_add_option(
    parser, 'o', "output", ARGPARSE_NARGS_1,
    ARGPARSE_TYPE_STRING, "Output file", "output");

argparse_option_set_default(opt, "a.out");
argparse_option_set_required(opt, true);
argparse_option_set_metavar(opt, "FILE");
```
