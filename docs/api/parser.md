# Parser API

Parser lifecycle and configuration functions.

## Functions

### `argparse_new`

```c
struct argparse *argparse_new(const char *prog, const char *description);
```

Create a new argument parser.

**Parameters:**
- `prog` — Program name for help output. NULL defaults to "program".
- `description` — Description shown in help. NULL for none.

**Returns:** Parser pointer, or NULL on allocation failure.

**Example:**
```c
struct argparse *parser = argparse_new("myapp", "My application");
```

### `argparse_free`

```c
void argparse_free(struct argparse *parser);
```

Free a parser and all associated resources. Safe to call with NULL.

### `argparse_set_prog`

```c
void argparse_set_prog(struct argparse *parser, const char *prog);
```

Set the program name displayed in help output.

### `argparse_set_description`

```c
void argparse_set_description(struct argparse *parser, const char *description);
```

Set the description shown in help output.

### `argparse_set_epilog`

```c
void argparse_set_epilog(struct argparse *parser, const char *epilog);
```

Set the epilog shown after the argument list.

### `argparse_set_usage`

```c
void argparse_set_usage(struct argparse *parser, const char *usage);
```

Override the auto-generated usage line.

### `argparse_set_prefix_chars`

```c
void argparse_set_prefix_chars(struct argparse *parser, const char *chars);
```

Set the characters that prefix optional arguments (default: "-").

### `argparse_set_fromfile_prefix_chars`

```c
void argparse_set_fromfile_prefix_chars(struct argparse *parser, const char *chars);
```

Set the from-file prefix characters (e.g., "@").

## Lifecycle

```
argparse_new()
    ↓
[configure: add options, set properties]
    ↓
argparse_parse()  ← can be called multiple times
    ↓
argparse_free()
```

## Thread Safety

- Parser creation and configuration is **not** thread-safe
- A parser can be parsed from multiple threads if not being modified
- Each thread should use its own `argparse_result`
