# Plugin System API Reference

Custom type handler registry for extending the type system.

## Functions

### `argparse_register_type`

```c
int argparse_register_type(const char *type_name,
                            argparse_type_parse_fn parse_fn);
```

Register a custom type handler globally. Returns 0 on success, -1 on failure. Maximum 32 registered types. Registering a duplicate name replaces the existing handler.

### `argparse_unregister_type`

```c
void argparse_unregister_type(const char *type_name);
```

Remove a custom type handler. No-op if the type is not registered.

## Types

### `argparse_type_parse_fn`

```c
typedef bool (*argparse_type_parse_fn)(const char *value, void *output,
                                       char *error_buf, size_t error_buf_size);
```

Parse callback for custom types. Returns `true` on success, `false` on error with message in `error_buf`.

## Constants

### `ARGPARSE_TYPE_CUSTOM_BASE`

```c
#define ARGPARSE_TYPE_CUSTOM_BASE 1000
```

Base value for custom type identifiers. Custom types should use IDs >= 1000 to avoid conflicts with built-in types.
