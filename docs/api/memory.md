# Memory API

Custom allocator support for memory management.

## Types

### `argparse_allocator`

```c
typedef struct argparse_allocator {
    argparse_alloc_fn alloc;
    argparse_free_fn free;
    void *user_data;
} argparse_allocator;
```

## Functions

### `argparse_default_allocator`

```c
const argparse_allocator *argparse_default_allocator(void);
```

Get the current default allocator.

### `argparse_set_default_allocator`

```c
void argparse_set_default_allocator(const argparse_allocator *allocator);
```

Set the default allocator for all subsequent parser operations.

**Warning:** Changing the allocator while parsers are active may cause undefined behavior.

### `argparse_alloc`

```c
void *argparse_alloc(size_t size);
```

Allocate memory using the current default allocator.

### `argparse_mem_free`

```c
void argparse_mem_free(void *ptr);
```

Free memory using the current default allocator.

## Example

```c
#include <argparse-c/argparse.h>
#include <stdlib.h>

/* Tracking allocator */
static size_t total_allocated = 0;

void *tracking_alloc(size_t size, void *user_data) {
    (void)user_data;
    total_allocated += size;
    return malloc(size);
}

void tracking_free(void *ptr, void *user_data) {
    (void)user_data;
    free(ptr);
}

int main(void) {
    argparse_allocator allocator = {
        .alloc = tracking_alloc,
        .free = tracking_free,
        .user_data = NULL
    };
    argparse_set_default_allocator(&allocator);

    /* All parser operations now use tracking_alloc/tracking_free */
    struct argparse *parser = argparse_new("app", NULL);
    /* ... */

    printf("Total allocated: %zu bytes\n", total_allocated);

    argparse_free(parser);
    return 0;
}
```

## Notes

- The default allocator uses standard `malloc`/`free`
- The allocator struct is copied internally
- Set the allocator before creating any parsers
- The `free` function must handle NULL pointers
