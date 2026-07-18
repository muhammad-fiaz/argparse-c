---
title: "Namespace / Result API"
description: "Parse arguments and access typed values from parse results."
---

# Namespace / Result API

Parsing arguments and accessing parsed values.

## Parsing

### `argparse_parse`

```c
struct argparse_result *argparse_parse(struct argparse *parser,
                                       int argc,
                                       const char **argv);
```

Parse command-line arguments.

**Returns:** Parse result. Caller must free with `argparse_result_free()`.

### `argparse_result_free`

```c
void argparse_result_free(struct argparse_result *result);
```

Free a parse result. Safe to call with NULL.

## Scalar Value Access

### `argparse_get_string`

```c
const char *argparse_get_string(const struct argparse_result *result,
                                const char *dest);
```

Get a string value. Returns NULL if not set.

### `argparse_get_int`

```c
int argparse_get_int(const struct argparse_result *result, const char *dest);
```

Get an integer value. Returns 0 if not set.

### `argparse_get_uint`

```c
unsigned int argparse_get_uint(const struct argparse_result *result,
                               const char *dest);
```

Get an unsigned integer. Returns 0 if not set.

### `argparse_get_float`

```c
float argparse_get_float(const struct argparse_result *result,
                         const char *dest);
```

Get a float value. Returns 0.0f if not set.

### `argparse_get_double`

```c
double argparse_get_double(const struct argparse_result *result,
                           const char *dest);
```

Get a double value. Returns 0.0 if not set.

### `argparse_get_bool`

```c
bool argparse_get_bool(const struct argparse_result *result, const char *dest);
```

Get a boolean value. Returns false if not set.

### `argparse_get_enum`

```c
const char *argparse_get_enum(const struct argparse_result *result,
                              const char *dest);
```

Get an enum string value. Returns NULL if not set.

## Array Value Access

### `argparse_get_count`

```c
size_t argparse_get_count(const struct argparse_result *result,
                          const char *dest);
```

Get the number of values for multi-value options.

### `argparse_get_string_at`

```c
const char *argparse_get_string_at(const struct argparse_result *result,
                                   const char *dest,
                                   size_t index);
```

Get a string at a specific index.

### `argparse_get_int_at`

```c
int argparse_get_int_at(const struct argparse_result *result,
                        const char *dest,
                        size_t index);
```

Get an integer at a specific index.

## Example

```c
struct argparse_result *result = argparse_parse(parser, argc, argv);

if (argparse_result_error_code(result) != ARGPARSE_OK) {
    fprintf(stderr, "Error: %s\n", argparse_result_error(result));
    argparse_result_free(result);
    return 1;
}

const char *name = argparse_get_string(result, "name");
int count = argparse_get_int(result, "count");
bool verbose = argparse_get_bool(result, "verbose");

argparse_result_free(result);
```
