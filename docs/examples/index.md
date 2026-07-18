---
title: "Examples"
description: "Browse example programs demonstrating argparse-c features. cmake -B build -DARGPARSE_BUILD_EXAMPLES=ON cmake --build build --parallel Each example is a complete, runnable program. Copy the code, compile, and run: gcc -o example examples/basic.c -Iinclude -Lbuild -largparse-c"
---

# Examples

Browse example programs demonstrating argparse-c features.

## Available Examples

| Example | Description | Difficulty |
|---------|-------------|------------|
| [Basic Usage](basic.md) | Simple option and positional parsing | Beginner |
| [Flags & Switches](flags.md) | Boolean flags and count options | Beginner |
| [Options & Types](options.md) | All option types with custom defaults | Beginner |
| [Enums](enums.md) | Restricted choice values with enums | Beginner |
| [Error Handling](error-handling.md) | Comprehensive error diagnostics | Beginner |
| [Environment & Config](env-config.md) | Env vars and config file loading | Intermediate |
| [Validators](validators.md) | Custom value validation | Intermediate |
| [Groups](groups.md) | Organized help output | Intermediate |
| [Arrays & Lists](arrays.md) | Multi-value arguments | Intermediate |
| [Subcommands](subcommands.md) | Multi-command CLI applications | Intermediate |
| [Advanced Features](advanced.md) | Callbacks, store_const, required options | Advanced |
| [C++ Integration](cpp.md) | Modern C++ usage patterns | Advanced |
| [Large CLI Apps](large-cli.md) | Real-world project structure | Advanced |

## Building Examples

```bash
cmake -B build -DARGPARSE_BUILD_EXAMPLES=ON
cmake --build build --parallel
```

## Quick Start

Each example is a complete, runnable program. Copy the code, compile, and run:

```bash
gcc -o example examples/basic.c -Iinclude -Lbuild -largparse-c
./example --help
```
