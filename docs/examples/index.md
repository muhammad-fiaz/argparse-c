# Examples

Browse example programs demonstrating argparse-c features.

## Available Examples

| Example | Description | Difficulty |
|---------|-------------|------------|
| [Basic Usage](basic.md) | Simple option and positional parsing | Beginner |
| [Flags & Switches](flags.md) | Boolean flags and count options | Beginner |
| [Subcommands](subcommands.md) | Multi-command CLI applications | Intermediate |
| [Validators](validators.md) | Custom value validation | Intermediate |
| [Groups](groups.md) | Organized help output | Intermediate |
| [Arrays & Lists](arrays.md) | Multi-value arguments | Intermediate |
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
