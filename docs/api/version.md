---
title: "Version API"
description: "Version information for the argparse-c library. const char *argparse_version_string(void); Get the library version as a string. void argparse_version(int *major, int *minor, int *patch); Get the library version as individual components. int main(void) { argparse-c follows [Semantic Versioning](https://semver.org/): Check compatibility at compile time:"
---

# Version API

Version information for the argparse-c library.

## Compile-Time Macros

```c
#define ARGPARSE_VERSION_MAJOR 0    /* Major version */
#define ARGPARSE_VERSION_MINOR 1    /* Minor version */
#define ARGPARSE_VERSION_PATCH 0    /* Patch version */
#define ARGPARSE_VERSION_STRING "0.1.0"  /* Full version string */

/* Compute version number: major * 10000 + minor * 100 + patch */
#define ARGPARSE_VERSION_NUM(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
```

## Functions

### `argparse_version_string`

```c
const char *argparse_version_string(void);
```

Get the library version as a string.

**Returns:** Static string (e.g., "0.1.0"). Must not be freed.

### `argparse_version`

```c
void argparse_version(int *major, int *minor, int *patch);
```

Get the library version as individual components.

**Parameters:**
- `major` — Pointer to receive major version, or NULL
- `minor` — Pointer to receive minor version, or NULL
- `patch` — Pointer to receive patch version, or NULL

## Example

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(void) {
    /* Runtime version */
    printf("Version: %s\n", argparse_version_string());

    int major, minor, patch;
    argparse_version(&major, &minor, &patch);
    printf("Version: %d.%d.%d\n", major, minor, patch);

    /* Compile-time version */
    printf("Compile-time: %s\n", ARGPARSE_VERSION_STRING);

    /* Version comparison */
    int current = ARGPARSE_VERSION_NUM(
        ARGPARSE_VERSION_MAJOR,
        ARGPARSE_VERSION_MINOR,
        ARGPARSE_VERSION_PATCH);

    int required = ARGPARSE_VERSION_NUM(0, 1, 0);
    if (current >= required) {
        printf("Version is compatible\n");
    }

    return 0;
}
```

## Version Compatibility

argparse-c follows [Semantic Versioning](https://semver.org/):

- **Major** — Incompatible API changes
- **Minor** — New functionality, backwards-compatible
- **Patch** — Bug fixes, backwards-compatible

Check compatibility at compile time:

```c
#if ARGPARSE_VERSION_NUM(ARGPARSE_VERSION_MAJOR, ARGPARSE_VERSION_MINOR, 0) < 100
#error "Requires argparse-c 0.1.0 or later"
#endif
```
