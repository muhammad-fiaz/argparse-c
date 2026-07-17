# Advanced Usage

This guide covers advanced features and patterns for power users.

## Table of Contents

- [From-File Expansion](#from-file-expansion)
- [Custom Error Handling](#custom-error-handling)
- [Reusable Parser Components](#reusable-parser-components)
- [Parser Inheritance](#parser-inheritance)
- [Dynamic Option Registration](#dynamic-option-registration)
- [Thread Safety](#thread-safety)
- [Embedding in Libraries](#embedding-in-libraries)
- [Testing Strategies](#testing-strategies)

## From-File Expansion

Enable `@file` expansion to read arguments from files:

```c
argparse_set_fromfile_prefix_chars(parser, "@");

struct argparse_result *result = argparse_parse(parser, argc, argv);
```

Now users can write:

```bash
myapp @args.txt
```

Where `args.txt` contains:

```
--output result.txt
--verbose
input.dat
```

Each line is treated as a separate argument. Lines starting with `#` are comments.

## Custom Error Handling

Override the default error behavior by checking error codes:

```c
struct argparse_result *result = argparse_parse(parser, argc, argv);
argparse_error_code code = argparse_result_error_code(result);

switch (code) {
case ARGPARSE_OK:
    /* Parse successful */
    break;

case ARGPARSE_ERROR_MISSING_ARGUMENT:
    fprintf(stderr, "Missing required argument: %s\n",
            argparse_result_error(result));
    argparse_print_usage(parser);
    break;

case ARGPARSE_ERROR_INVALID_VALUE:
    fprintf(stderr, "Invalid value: %s\n",
            argparse_result_error(result));
    break;

case ARGPARSE_ERROR_CONFLICT:
    fprintf(stderr, "Conflicting options: %s\n",
            argparse_result_error(result));
    break;

default:
    fprintf(stderr, "Parse error (%d): %s\n", code,
            argparse_result_error(result));
    break;
}

argparse_result_free(result);
```

## Reusable Parser Components

Create modular parser components that can be combined:

```c
/* common_options.h - Shared option setup */
void add_common_options(struct argparse *parser) {
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose output", "verbose");
    argparse_add_option(parser, 'q', "quiet", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Quiet mode", "quiet");
    argparse_add_option(parser, 0, "log-level", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Log level", "log_level");
}

/* main.c - Use the shared setup */
int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("myapp", "My application");
    add_common_options(parser);
    /* Add app-specific options... */
}
```

## Dynamic Option Registration

Register options based on runtime configuration:

```c
void add_dynamic_options(struct argparse *parser, const char **option_names,
                         size_t count) {
    for (size_t i = 0; i < count; i++) {
        char dest[64];
        snprintf(dest, sizeof(dest), "opt_%zu", i);
        argparse_add_option(parser, 0, option_names[i], ARGPARSE_NARGS_1,
                            ARGPARSE_TYPE_STRING, NULL, dest);
    }
}
```

## Thread Safety

argparse-c parser instances are **not thread-safe**. For concurrent use:

1. **Separate instances**: Each thread should have its own parser
2. **Immutable parsing**: Once created, parsers can be shared if read-only
3. **Results are independent**: Each `argparse_result` is independent

```c
/* Thread-safe pattern */
void *thread_func(void *arg) {
    struct argparse *my_parser = argparse_new("worker", "Worker thread");
    /* Each thread creates its own parser */
    argparse_add_option(my_parser, 'i', "id", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Worker ID", "id");

    struct argparse_result *result = argparse_parse(my_parser, argc, argv);
    /* Process result... */
    argparse_result_free(result);
    argparse_free(my_parser);
    return NULL;
}
```

## Embedding in Libraries

When using argparse-c in a library:

1. **Don't install parsers globally** — each library component should own its parser
2. **Use namespaced destinations** — avoid name collisions with `prefix_option`
3. **Hide implementation details** — use opaque pointers in your public API

```c
/* mylib.h */
typedef struct mylib_config mylib_config;

mylib_config *mylib_config_parse(int argc, const char **argv);
void mylib_config_free(mylib_config *config);

/* mylib.c */
struct mylib_config {
    struct argparse_result *result;
};

mylib_config *mylib_config_parse(int argc, const char **argv) {
    struct argparse *parser = argparse_new("mylib", "My library");
    /* Configure options with "mylib_" prefix */
    argparse_add_option(parser, 'v', "mylib-verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose", "mylib_verbose");

    struct argparse_result *result = argparse_parse(parser, argc, argv);
    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        argparse_result_free(result);
        argparse_free(parser);
        return NULL;
    }

    mylib_config *config = malloc(sizeof(mylib_config));
    config->result = result;
    argparse_free(parser);  /* Parser can be freed after parsing */
    return config;
}
```

## Testing Strategies

### Unit Testing Parsers

```c
#include <assert.h>
#include <argparse-c/argparse.h>

void test_verbose_flag(void) {
    struct argparse *parser = argparse_new("test", NULL);
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, NULL, "verbose");

    const char *argv[] = {"test", "--verbose"};
    struct argparse_result *result = argparse_parse(parser, 2, argv);

    assert(argparse_result_error_code(result) == ARGPARSE_OK);
    assert(argparse_get_bool(result, "verbose") == true);

    argparse_result_free(result);
    argparse_free(parser);
}
```

### Testing Error Cases

```c
void test_missing_required(void) {
    struct argparse *parser = argparse_new("test", NULL);
    struct argparse_option *opt = argparse_add_positional(
        parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, NULL, "input");
    argparse_option_set_required(opt, true);

    const char *argv[] = {"test"};
    struct argparse_result *result = argparse_parse(parser, 1, argv);

    assert(argparse_result_error_code(result) == ARGPARSE_ERROR_MISSING_ARGUMENT);

    argparse_result_free(result);
    argparse_free(parser);
}
```

### Testing with Temporary Config Files

```c
void test_config_loading(void) {
    /* Create temp config file */
    FILE *f = fopen("test.conf", "w");
    fprintf(f, "output = result.txt\nverbose = true\n");
    fclose(f);

    struct argparse *parser = argparse_new("test", NULL);
    argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, NULL, "output");
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, NULL, "verbose");

    argparse_error_code err = argparse_load_config(parser, "test.conf");
    assert(err == ARGPARSE_OK);

    /* Parse without command-line args (uses config values) */
    const char *argv[] = {"test"};
    struct argparse_result *result = argparse_parse(parser, 1, argv);

    assert(strcmp(argparse_get_string(result, "output"), "result.txt") == 0);
    assert(argparse_get_bool(result, "verbose") == true);

    argparse_result_free(result);
    argparse_free(parser);
    remove("test.conf");
}
```
