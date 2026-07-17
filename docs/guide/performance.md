# Performance

Performance characteristics and optimization tips for argparse-c.

## Benchmarks

Run the built-in benchmarks:

```bash
cmake -B build -DARGPARSE_BUILD_BENCHMARKS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/benchmarks/bench_argparse
```

### Typical Results

| Operation | Time |
|-----------|------|
| Parser creation (5 options) | ~30 μs |
| Parsing 6 arguments | ~5 μs |
| Parsing 20 options | ~15 μs |
| Parser with subcommands | ~8 μs |

## Performance Characteristics

- **Parser creation**: O(n) where n is the number of options
- **Parsing**: O(n) where n is the number of arguments
- **Memory**: O(n) where n is the number of options + parsed values
- **String handling**: Zero-copy where possible

## Optimization Tips

### 1. Reuse Parsers

Create the parser once and reuse it:

```c
/* Good: create once */
struct argparse *parser = create_parser();

for (int i = 0; i < batch_size; i++) {
    struct argparse_result *result = argparse_parse(parser, argc, argv);
    process(result);
    argparse_result_free(result);
}

argparse_free(parser);

/* Bad: creating parser for each parse */
for (int i = 0; i < batch_size; i++) {
    struct argparse *parser = create_parser(); /* Wasteful! */
    struct argparse_result *result = argparse_parse(parser, argc, argv);
    argparse_result_free(result);
    argparse_free(parser);
}
```

### 2. Use Static Allocators

For batch processing, use an arena allocator:

```c
static char arena[1024 * 1024];
static size_t arena_offset = 0;

void *arena_alloc(size_t size, void *user_data) {
    (void)user_data;
    size = (size + 7) & ~(size_t)7;
    if (arena_offset + size > sizeof(arena)) return NULL;
    void *ptr = &arena[arena_offset];
    arena_offset += size;
    return ptr;
}

void arena_free(void *ptr, void *user_data) {
    (void)ptr; (void)user_data;
}
```

### 3. Minimize Option Count

Fewer options = faster parsing. Consider grouping related options or using subcommands.

### 4. Use Short Options

Short options (`-v`) are slightly faster to match than long options (`--verbose`).

## Memory Usage

- Each parser: ~512 bytes base + ~128 bytes per option
- Each parse result: ~256 bytes base + ~64 bytes per parsed value
- Strings are copied into the result; no pointers to original argv

## Comparison with Manual Parsing

argparse-c adds ~2-5 μs overhead compared to hand-written `getopt`-style parsing for simple cases. This overhead is negligible for most applications and provides significant benefits:

- Type checking and conversion
- Help generation
- Error handling
- Unicode support
- Config file support

For extremely performance-critical paths (parsing millions of argument sets), consider manual parsing and validate with argparse-c for user-facing interfaces.
