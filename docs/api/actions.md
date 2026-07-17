# Actions API

Option actions determine how parsed values are stored or what side effects occur.

## Available Actions

| Action | Description |
|--------|-------------|
| `ARGPARSE_STORE` | Store the value (default) |
| `ARGPARSE_STORE_CONST` | Store a constant value |
| `ARGPARSE_STORE_TRUE` | Store `true` (flag) |
| `ARGPARSE_STORE_FALSE` | Store `false` (flag) |
| `ARGPARSE_APPEND` | Append value to a list |
| `ARGPARSE_COUNT` | Increment a counter |
| `ARGPARSE_HELP` | Print help and exit |
| `ARGPARSE_VERSION` | Print version and exit |
| `ARGPARSE_EXTEND` | Extend list with multiple values |

## Usage

### Store (Default)

```c
argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_STRING, "Output file", "output");
```

### Store True / Store False

```c
/* --verbose sets verbose to true */
argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Verbose output", "verbose");

/* --no-color sets color to false */
argparse_add_option(parser, 0, "no-color", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Disable color", "color");
argparse_option_set_action(/* ... */, ARGPARSE_STORE_FALSE);
```

### Store Const

```c
struct argparse_option *opt = argparse_add_option(
    parser, 0, "mode", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE,
    "Special mode", "mode");
argparse_option_set_action(opt, ARGPARSE_STORE_CONST);
argparse_option_set_const(opt, "special");
```

### Append

```c
argparse_add_option(parser, 'I', "include", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_STRING, "Include path", "includes");
argparse_option_set_action(/* ... */, ARGPARSE_APPEND);

/* -I foo -I bar → includes = ["foo", "bar"] */
```

### Count

```c
argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Verbosity level", "verbose");
argparse_option_set_action(/* ... */, ARGPARSE_COUNT);

/* -v → 1, -vv → 2, -vvv → 3 */
```

### Extend

```c
argparse_add_option(parser, 'e', "exclude", ARGPARSE_NARGS_STAR,
                    ARGPARSE_TYPE_STRING, "Exclude patterns", "excludes");
argparse_option_set_action(/* ... */, ARGPARSE_EXTEND);

/* --exclude foo bar baz → excludes = ["foo", "bar", "baz"] */
```

### Help / Version

```c
argparse_add_option(parser, 'h', "help", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Show help", NULL);
argparse_option_set_action(/* ... */, ARGPARSE_HELP);

argparse_add_option(parser, 'V', "version", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Show version", NULL);
argparse_option_set_action(/* ... */, ARGPARSE_VERSION);
```
