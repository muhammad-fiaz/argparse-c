# Thread Safety

argparse-c supports thread-safe parsing through its freeze/unfreeze mechanism. A frozen parser can be safely shared across multiple threads for concurrent read-only parsing.

## How It Works

1. Create and configure a parser normally (single-threaded)
2. Freeze the parser when configuration is complete
3. Use the frozen parser from multiple threads concurrently
4. Each thread gets its own independent parse result

## Example

```c
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <thread>

struct argparse *parser = argparse_new("myapp", "Thread-safe CLI");

argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Verbose output", NULL);
argparse_add_option(parser, 'n', "count", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_INT, "Count", "count");

// Freeze: no more modifications allowed after this point
argparse_freeze(parser);

// Now safe to use from multiple threads
const char *argv[] = {"myapp", "-v", "-n", "42"};

// Each thread gets independent results
struct argparse_result *result = argparse_parse_threadsafe(parser, 4, argv);
if (result) {
    printf("Thread %d: verbose=%d, count=%d\n",
           thread_id,
           argparse_get_bool(result, "verbose"),
           argparse_get_int(result, "count"));
    argparse_result_free(result);
}
```

## API

### `argparse_freeze(parser)`

Mark a parser as frozen (read-only). After this call, the parser must not be modified.

### `argparse_is_frozen(parser)`

Check if a parser is frozen. Returns `true` if frozen.

### `argparse_parse_threadsafe(parser, argc, argv)`

Thread-safe parse using a frozen parser. Behaves identically to `argparse_parse()` but documents the frozen requirement. Multiple threads may call this concurrently on the same frozen parser.

## Thread Safety Rules

- **Before freeze**: Parser must only be accessed from a single thread
- **After freeze**: Parser can be shared read-only across threads
- **Results**: Each `argparse_parse_threadsafe()` call returns an independent result
- **Freeing**: Results and parsers must be freed by the thread that owns them

## Important Notes

- Freezing is irreversible for the parser's lifetime
- Attempting to modify a frozen parser leads to undefined behavior
- `argparse_freeze()` is not thread-safe — call it before concurrent use
- `argparse_is_frozen()` is safe to call concurrently
