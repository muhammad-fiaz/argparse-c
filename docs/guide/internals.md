---
title: "Internals"
description: "Internal architecture and implementation details"
---

# Internals

Technical details about how argparse-c works internally.

## Architecture

argparse-c is organized into modular components:

```
src/
├── argparse_parser.c      # Core parser lifecycle
├── argparse_argument.c    # Option and positional creation
├── argparse_namespace.c   # Parse result and value access
├── argparse_action.c      # Action implementations
├── argparse_validator.c   # Validation framework
├── argparse_group.c       # Argument groups
├── argparse_mutex.c       # Mutually exclusive groups
├── argparse_command.c     # Subcommand support
├── argparse_format.c      # Help output formatting
├── argparse_error.c       # Error handling
├── argparse_memory.c      # Custom allocator support
├── argparse_config.c      # Config file loading
├── argparse_complete.c    # Shell completion generation
├── argparse_unicode.c     # UTF-8 support
├── argparse_version.c     # Version information
├── argparse_internal.c    # Internal utilities
└── argparse_internal.h    # Internal data structures
```

## Data Structures

### Parser (`struct argparse`)

The parser is the central object. It holds:

- Program name and description
- List of options and positional arguments
- List of groups
- List of subcommands
- Configuration (prefix chars, env prefix, etc.)

### Option (`struct argparse_option`)

Each option stores:

- Short and long names
- Nargs, type, action
- Default and const values
- Choices (for enum type)
- Callback function and data
- Validator function
- Help text and metavar
- Required flag

### Result (`struct argparse_result`)

The parse result is an immutable snapshot of parsed values:

- Map of destination names to values
- Error code and message
- Subcommand information

## Parsing Algorithm

1. **Initialization**: Set all options to their default values
2. **Iteration**: Walk through argv one token at a time
3. **Option matching**: Match tokens against short/long option names
4. **Value consumption**: Based on nargs, consume the appropriate number of following tokens
5. **Type conversion**: Convert string values to the option's type
6. **Validation**: Run built-in validation (type, choices) and custom validators
7. **Storage**: Store the converted value in the result namespace
8. **Constraint checking**: Verify mutually exclusive groups and required options
9. **Result creation**: Build the immutable result object

## Memory Management

argparse-c uses a two-phase memory model:

1. **Parser lifetime**: All parser configuration (options, groups, etc.) lives for the parser's lifetime
2. **Result lifetime**: Parse results are independent objects with their own memory

Strings are always copied, so the parser and result don't hold pointers into argv.

## Thread Safety

- Parser creation/modification is **not** thread-safe
- Parse results are **safe to read** concurrently
- The default allocator is **global state** — set before creating parsers

## Error Handling

Errors are stored in the parse result rather than using longjmp or global errno. This allows:

- Clean error propagation
- Multiple error details
- Thread-safe error handling

## Future Considerations

- Compile-time option registration (C11 `_Generic`)
- Arena-based allocation for zero heap usage
- Streaming parse mode for very large argument lists
