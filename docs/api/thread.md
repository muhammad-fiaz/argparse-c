---
title: "Thread Safety API Reference"
description: "Thread-safe parsing through freeze/unfreeze mechanism. void argparse_freeze(struct argparse *parser); Mark a parser as frozen (read-only). Once frozen, the parser configuration must not be modified. This enables safe concurrent use across multiple threads. bool argparse_is_frozen(const struct argparse *parser); Check if a parser is frozen. Safe to call concurrently. struct argparse_result *argparse_parse_threadsafe(struct argparse *parser, Thread-safe parse using a frozen parser. Behaves identically to `argparse_parse()` but requires the parser to be frozen. Multiple threads may call this concurrently, each receiving independent results."
---

# Thread Safety API Reference

Thread-safe parsing through freeze/unfreeze mechanism.

## Functions

### `argparse_freeze`

```c
void argparse_freeze(struct argparse *parser);
```

Mark a parser as frozen (read-only). Once frozen, the parser configuration must not be modified. This enables safe concurrent use across multiple threads.

### `argparse_is_frozen`

```c
bool argparse_is_frozen(const struct argparse *parser);
```

Check if a parser is frozen. Safe to call concurrently.

### `argparse_parse_threadsafe`

```c
struct argparse_result *argparse_parse_threadsafe(struct argparse *parser,
                                                   int argc,
                                                   const char **argv);
```

Thread-safe parse using a frozen parser. Behaves identically to `argparse_parse()` but requires the parser to be frozen. Multiple threads may call this concurrently, each receiving independent results.

**Returns:** A newly allocated parse result, or NULL on allocation failure. Caller must free with `argparse_result_free()`.
