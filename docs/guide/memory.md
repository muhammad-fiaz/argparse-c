---
title: Memory Management
description: Proper memory management patterns
---

# Memory Management

argparse-c provides clean memory management with explicit allocation and deallocation.

## Memory Lifecycle

```
argparse_new()  →  argparse_add_*()  →  argparse_parse()  →  argparse_free()
                                                        ↓
                                            argparse_result_free()
```

## Basic Pattern

```c
/* Create parser */
struct argparse *parser = argparse_new("myapp", "Description");

/* Add arguments */
argparse_add_option(parser, ...);
argparse_add_positional(parser, ...);

/* Parse */
struct argparse_result *result = argparse_parse(parser, argc, argv);

/* Use results */
const char *val = argparse_get_string(result, "name");

/* Cleanup */
argparse_result_free(result);
argparse_free(parser);
```

## RAII Pattern (C++)

```cpp
#include <argparse-c/argparse.h>
#include <memory>

struct argparse_deleter {
    void operator()(struct argparse *p) const { argparse_free(p); }
};

struct result_deleter {
    void operator()(struct argparse_result *r) const { argparse_result_free(r); }
};

int main(int argc, char *argv[]) {
    std::unique_ptr<struct argparse, argparse_deleter> parser(
        argparse_new("myapp", "Description")
    );

    argparse_add_option(parser.get(), 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose", NULL);

    std::unique_ptr<struct argparse_result, result_deleter> result(
        argparse_parse(parser.get(), argc, (const char **)argv)
    );

    if (argparse_result_error_code(result.get()) != ARGPARSE_OK) {
        fprintf(stderr, "%s\n", argparse_result_error(result.get()));
        return 1;
    }

    /* Automatic cleanup */
    return 0;
}
```

## Error-Safe Pattern

```c
int main(int argc, char *argv[]) {
    struct argparse *parser = NULL;
    struct argparse_result *result = NULL;
    int exit_code = 0;

    parser = argparse_new("myapp", "Description");
    if (!parser) {
        fprintf(stderr, "Failed to create parser\n");
        return 1;
    }

    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose", NULL);

    result = argparse_parse(parser, argc, (const char **)argv);
    if (!result) {
        fprintf(stderr, "Failed to parse arguments\n");
        exit_code = 1;
        goto cleanup;
    }

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        exit_code = 1;
        goto cleanup;
    }

    if (argparse_result_should_exit(result)) {
        exit_code = argparse_result_exit_code(result);
        goto cleanup;
    }

    /* Use results */
    printf("Verbose: %s\n",
           argparse_get_bool(result, "verbose") ? "yes" : "no");

cleanup:
    if (result) argparse_result_free(result);
    if (parser) argparse_free(parser);
    return exit_code;
}
```

## Memory Safety

### No Double Free

```c
argparse_result_free(result);
/* Do NOT call argparse_result_free(result) again */
result = NULL;  /* Set to NULL after freeing */
```

### No Use After Free

```c
argparse_result_free(result);
/* Do NOT use result after freeing */
```

### No Leaks

```c
/* Always free both result and parser */
argparse_result_free(result);
argparse_free(parser);
```

## Custom Allocators

For specialized memory management:

```c
/* See Custom Allocators guide */
```

## Next Steps

- [Custom Allocators](./custom-allocators) - Custom memory management
- [API: Memory](../api/memory) - Complete API reference
