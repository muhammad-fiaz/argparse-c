---
title: Installation
description: How to install and integrate argparse-c into your project
---

# Installation

argparse-c is designed for easy integration. Choose the method that works best for your project.

## Requirements

- **C compiler** supporting C99 or later (GCC, Clang, MSVC)
- **C++ compiler** supporting C++11 or later (for C++ bindings)
- No external dependencies

## Method 1: Copy Source Files (Recommended)

The simplest approach — copy the source files directly into your project:

```
your-project/
├── include/
│   └── argparse-c/
│       └── argparse.h
└── src/
    └── argparse.c
```

Then compile alongside your project:

```bash
gcc -o myapp main.c src/argparse.c -Iinclude
```

## Method 2: Git Submodule

Add argparse-c as a git submodule:

```bash
git submodule add https://github.com/muhammad-fiaz/argparse-c.git vendor/argparse-c
git submodule update --init --recursive
```

Compile with:

```bash
gcc -o myapp main.c vendor/argparse-c/src/argparse.c \
    -Ivendor/argparse-c/include
```

## Method 3: CMake

If your project uses CMake, add argparse-c as a subdirectory:

```cmake
# In your CMakeLists.txt
add_subdirectory(vendor/argparse-c)
target_link_libraries(myapp PRIVATE argparse-c)
```

Then in your vendor directory:

```bash
cd vendor
git clone https://github.com/muhammad-fiaz/argparse-c.git
```

## Method 4: Makefile

For Makefile-based projects:

```makefile
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Iinclude
SOURCES = main.c vendor/argparse-c/src/argparse.c

myapp: $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f myapp
```

## Method 5: Meson

```ini
# meson.build
project('myapp', 'c', version: '1.0')

argparse_dep = subproject('argparse-c')
executable('myapp', 'main.c', dependencies: argparse_dep.get_dependency('argparse-c'))
```

## Verifying Installation

Create a minimal test program:

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(void) {
    printf("argparse-c version: %s\n", argparse_version_string());
    return 0;
}
```

Compile and run:

```bash
gcc -o test test.c src/argparse.c -Iinclude
./test
# Output: argparse-c version: 0.1.0
```

## Platform Notes

### Windows (MSVC)

```cmd
cl /std:c11 /I include main.c src\argparse.c
```

### macOS

```bash
clang -o myapp main.c src/argparse.c -Iinclude
```

### Cross-Compilation

```bash
arm-none-eabi-gcc -o myapp main.c src/argparse.c -Iinclude
```

## Next Steps

- [Quick Start](./quick-start) - Build your first CLI app
- [Parser](./parser) - Learn about parser configuration
