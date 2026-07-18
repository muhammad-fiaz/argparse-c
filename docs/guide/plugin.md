# Plugin System

The plugin system allows you to register custom type handlers that extend the built-in type system. Custom types can be used with any parser after registration.

## Registering a Custom Type

```c
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <stdlib.h>

// Custom parse function for IP addresses
static bool parse_ip(const char *value, void *output,
                     char *error_buf, size_t error_buf_size) {
    int a, b, c, d;
    if (sscanf(value, "%d.%d.%d.%d", &a, &b, &c, &d) != 4 ||
        a < 0 || a > 255 || b < 0 || b > 255 ||
        c < 0 || c > 255 || d < 0 || d > 255) {
        snprintf(error_buf, error_buf_size, "invalid IP address: %s", value);
        return false;
    }
    // Store as a simple struct or string
    *(const char **)output = value;
    return true;
}

int main(int argc, const char **argv) {
    // Register the custom type globally
    argparse_register_type("ip", parse_ip);

    struct argparse *parser = argparse_new("netapp", "Network application");

    // Use the custom type with ARGPARSE_TYPE_CUSTOM_BASE + offset
    argparse_add_option(parser, 'b', "bind", ARGPARSE_NARGS_1,
                        (argparse_type)(ARGPARSE_TYPE_CUSTOM_BASE),
                        "Bind address", "IP");

    struct argparse_result *result = argparse_parse(parser, argc, argv);
    // ... use results ...
    argparse_result_free(result);
    argparse_free(parser);

    // Clean up
    argparse_unregister_type("ip");
    return 0;
}
```

## Custom Type IDs

Built-in types occupy IDs 0-7. Custom types should use IDs starting at `ARGPARSE_TYPE_CUSTOM_BASE` (1000) to avoid conflicts:

```c
#define ARGPARSE_TYPE_CUSTOM_BASE 1000
```

## API Reference

### `argparse_register_type(type_name, parse_fn)`

Register a custom type handler globally.

| Parameter | Description |
|-----------|-------------|
| `type_name` | Name for the type (copied internally) |
| `parse_fn` | Parse function called during parsing |

Returns `0` on success, `-1` on failure (registry full or allocation error).

**Maximum registered types:** 32

### `argparse_unregister_type(type_name)`

Remove a custom type handler. No-op if the type is not registered.

## Parse Function Signature

```c
typedef bool (*argparse_type_parse_fn)(const char *value, void *output,
                                       char *error_buf, size_t error_buf_size);
```

| Parameter | Description |
|-----------|-------------|
| `value` | String value to parse |
| `output` | Pointer to output buffer for parsed value |
| `error_buf` | Buffer for error message on failure |
| `error_buf_size` | Size of error buffer |

Returns `true` on success, `false` on error (with message written to `error_buf`).

## Notes

- Custom types are registered globally, not per-parser
- Registering a type with an existing name replaces the old handler
- The maximum number of registered custom types is 32
- Registration is not thread-safe
