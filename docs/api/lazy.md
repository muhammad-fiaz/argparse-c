# Lazy Defaults API Reference

Lazy default value evaluation for options.

## Functions

### `argparse_option_set_lazy_default`

```c
void argparse_option_set_lazy_default(struct argparse_option *option,
                                      argparse_lazy_default_fn fn,
                                      void *user_data);
```

Set a lazy default value function on an option. The function is called during apply_defaults (after parsing) for any option that was not explicitly set on the command line.

| Parameter | Description |
|-----------|-------------|
| `option` | The option to configure |
| `fn` | The lazy default function, or `NULL` to clear |
| `user_data` | Opaque pointer passed to `fn` on each invocation |

## Types

### `argparse_lazy_default_fn`

```c
typedef const char *(*argparse_lazy_default_fn)(void *user_data);
```

Function that returns a default value string. Called only when the option was not provided on the command line.

## Behavior

- Lazy defaults are evaluated after parsing
- If an option was explicitly set, the lazy function is not called
- Lazy defaults take precedence over static defaults
- Setting `fn` to `NULL` clears the lazy default
