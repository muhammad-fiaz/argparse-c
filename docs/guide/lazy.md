# Lazy Defaults

Lazy defaults allow you to compute default values on demand at parse time, rather than specifying static defaults upfront. This is useful when the default depends on runtime state like environment variables, the current directory, or other options.

## How It Works

When an option is not provided on the command line, the lazy default function is called to compute the value. The function receives user data (set at registration time) and returns a string that is used as the option's value.

## Example

```c
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <stdlib.h>

// Lazy default that reads from an environment variable
static const char *env_default(void *user_data) {
    const char *env_name = (const char *)user_data;
    const char *val = getenv(env_name);
    return val ? val : "unknown";
}

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("myapp", "App with lazy defaults");

    struct argparse_option *opt = argparse_add_option(
        parser, 'o', "output", ARGPARSE_NARGS_1,
        ARGPARSE_TYPE_STRING, "Output file", "output");

    // Set lazy default: falls back to OUTPUT_FILE env var
    argparse_option_set_lazy_default(opt, env_default, "OUTPUT_FILE");

    struct argparse_result *result = argparse_parse(parser, argc, argv);

    // If --output was not given, the lazy function was called
    const char *output = argparse_get_string(result, "output");
    printf("Output: %s\n", output ? output : "(none)");

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Behavior

- Lazy defaults are evaluated **after** parsing, during the apply_defaults phase
- If an option was explicitly set on the command line, the lazy function is **not** called
- If both a static default and a lazy default are set, the lazy default takes precedence
- Setting `fn` to `NULL` clears the lazy default

## API

```c
void argparse_option_set_lazy_default(struct argparse_option *option,
                                      argparse_lazy_default_fn fn,
                                      void *user_data);
```

| Parameter | Description |
|-----------|-------------|
| `option` | The option to configure |
| `fn` | The lazy default function, or `NULL` to clear |
| `user_data` | Opaque pointer passed to `fn` on each invocation |

The lazy default function signature is:

```c
typedef const char *(*argparse_lazy_default_fn)(void *user_data);
```
