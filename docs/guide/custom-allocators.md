# Custom Allocators

argparse-c supports pluggable memory allocators, allowing you to replace the default `malloc`/`free` with custom implementations.

## Why Custom Allocators?

- **Embedded systems** — use pool allocators or static memory
- **Debugging** — track allocations and detect leaks
- **Performance** — use arena allocators for batch processing
- **Integration** — match your application's memory management

## Setting a Custom Allocator

```c
#include <argparse-c/argparse.h>

/* Example: arena allocator */
static size_t arena_offset = 0;
static char arena_buffer[1024 * 1024]; /* 1 MB arena */

void *arena_alloc(size_t size, void *user_data) {
    (void)user_data;
    /* Align to 8 bytes */
    size = (size + 7) & ~(size_t)7;

    if (arena_offset + size > sizeof(arena_buffer)) {
        return NULL; /* Arena exhausted */
    }

    void *ptr = &arena_buffer[arena_offset];
    arena_offset += size;
    return ptr;
}

void arena_free(void *ptr, void *user_data) {
    (void)ptr;
    (void)user_data;
    /* No-op: arena is freed all at once */
}

int main(void) {
    /* Set up the allocator before creating any parsers */
    argparse_allocator allocator = {
        .alloc = arena_alloc,
        .free = arena_free,
        .user_data = NULL
    };
    argparse_set_default_allocator(&allocator);

    /* All subsequent parser operations use the arena */
    struct argparse *parser = argparse_new("app", "Arena-allocated parser");
    /* ... use parser ... */
    argparse_free(parser);

    /* Reset arena for next batch */
    arena_offset = 0;

    return 0;
}
```

## Allocator Interface

```c
typedef struct argparse_allocator {
    argparse_alloc_fn alloc;    /* Must not be NULL */
    argparse_free_fn free;      /* Must not be NULL */
    void *user_data;            /* Passed to alloc and free */
} argparse_allocator;
```

### `alloc` Function

```c
typedef void *(*argparse_alloc_fn)(size_t size, void *user_data);
```

- `size` — Number of bytes to allocate (always > 0)
- `user_data` — Opaque pointer from the allocator struct
- Returns: Pointer to allocated memory, or NULL on failure

### `free` Function

```c
typedef void (*argparse_free_fn)(void *ptr, void *user_data);
```

- `ptr` — Pointer to free (may be NULL)
- `user_data` — Opaque pointer from the allocator struct

## Querying the Current Allocator

```c
const argparse_allocator *current = argparse_default_allocator();
printf("Current allocator uses: %s\n",
       current->alloc == my_alloc ? "custom" : "default");
```

## Thread Safety

The default allocator is **global state**. Changing it while parsers are active may cause undefined behavior. Set the allocator before creating any parsers.

## Debugging Allocator Example

```c
static size_t total_allocated = 0;
static size_t total_freed = 0;
static size_t allocation_count = 0;

void *debug_alloc(size_t size, void *user_data) {
    (void)user_data;
    allocation_count++;
    total_allocated += size;
    void *ptr = malloc(size);
    printf("[ALLOC] %zu bytes at %p (total: %zu)\n", size, ptr, total_allocated);
    return ptr;
}

void debug_free(void *ptr, void *user_data) {
    (void)user_data;
    if (ptr) {
        total_freed += 1; /* Can't know exact size without tracking */
        printf("[FREE]  %p\n", ptr);
    }
    free(ptr);
}

void print_memory_stats(void) {
    printf("\n=== Memory Statistics ===\n");
    printf("Allocations: %zu\n", allocation_count);
    printf("Total allocated: %zu bytes\n", total_allocated);
    printf("Total freed: %zu calls\n", total_freed);
}
```
