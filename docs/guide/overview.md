---
title: Overview
description: High-level overview of argparse-c architecture and concepts
---

# Overview

This page provides a high-level overview of how argparse-c works and its core concepts.

## Architecture

argparse-c follows a simple architecture:

```
┌─────────────┐     ┌──────────────┐     ┌─────────────────┐
│   Program   │────▶│    Parser    │────▶│     Result      │
│  (argc/argv)│     │  (defines)   │     │  (parsed args)  │
└─────────────┘     └──────────────┘     └─────────────────┘
                           │
                    ┌──────┴──────┐
                    ▼             ▼
              ┌──────────┐ ┌──────────┐
              │ Options  │ │Positional│
              └──────────┘ └──────────┘
```

## Core Concepts

### Parser

The parser is the central object. It defines:
- Program name and description
- Available options and positional arguments
- Parsing behavior and formatting

```c
struct argparse *parser = argparse_new("myapp", "My application");
```

### Options

Options are named flags that modify behavior:

```c
// Boolean flag: -v, --verbose
argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Enable verbose mode", NULL);

// String option: -o, --output FILE
argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_STRING, "Output file", "FILE");
```

### Positional Arguments

Positional arguments are identified by their position:

```c
// Required input file
argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                        "Input file", "INPUT");

// Optional output file
argparse_add_positional(parser, ARGPARSE_NARGS_0, ARGPARSE_TYPE_STRING,
                        "Output file (default: stdout)", "OUTPUT");
```

### Result

The result contains all parsed values:

```c
struct argparse_result *result = argparse_parse(parser, argc, argv);

// Check for errors
if (argparse_result_error_code(result) != ARGPARSE_OK) {
    fprintf(stderr, "%s\n", argparse_result_error(result));
}

// Get values
const char *input = argparse_get_string(result, "INPUT");
int verbose = argparse_get_bool(result, "verbose");
```

## Data Flow

1. **Define** - Create parser and add options/positionals
2. **Parse** - Pass command-line arguments to the parser
3. **Extract** - Get typed values from the result
4. **Cleanup** - Free the result and parser

## Lifecycle

```c
// 1. Create parser
struct argparse *parser = argparse_new("prog", "Description");

// 2. Add arguments
argparse_add_option(parser, ...);
argparse_add_positional(parser, ...);

// 3. Parse
struct argparse_result *result = argparse_parse(parser, argc, argv);

// 4. Use results
const char *val = argparse_get_string(result, "name");

// 5. Cleanup
argparse_result_free(result);
argparse_free(parser);
```

## Key Design Decisions

| Decision | Rationale |
|----------|-----------|
| C99 only | Maximum portability |
| Heap allocation | Supports complex argument structures |
| Result object | Clean separation of parsing and values |
| Named arguments | Intuitive access by name, not index |
| Auto help/version | Standard behavior, no boilerplate |

## Comparison with Other Libraries

| Feature | argparse-c | getopt | docopt.c |
|---------|------------|--------|----------|
| Auto help generation | ✅ | ❌ | ✅ |
| Type validation | ✅ | ❌ | ❌ |
| Subcommands | ✅ | ❌ | ✅ |
| Shell completion | ✅ | ❌ | ❌ |
| Zero dependencies | ✅ | ✅ | ❌ |
| C++ support | ✅ | ❌ | ❌ |

## Next Steps

- [Parser](./parser) - Configure parser behavior
- [Arguments](./arguments) - Define argument types
- [API Reference](../api/overview) - Complete API documentation
