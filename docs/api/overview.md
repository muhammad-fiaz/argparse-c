---
title: "API Overview"
description: "Complete reference for the public argparse-c API."
---

# API Overview

Complete reference for the argparse-c public API.

## Headers

```c
#include <argparse-c/argparse.h>  /* Convenience header */
```

Or include individual headers for what you need.

## Quick Reference

### Creating a Parser

```c
struct argparse *parser = argparse_new("prog", "Description");
```

### Adding Options

```c
/* Short option only */
argparse_add_option(parser, 'v', NULL, ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Verbose", "verbose");

/* Long option only */
argparse_add_option(parser, 0, "output", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_STRING, "Output file", "output");

/* Both short and long */
argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_STRING, "Output file", "output");
```

### Adding Positional Arguments

```c
argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                        "Input file", "input");
```

### Parsing

```c
struct argparse_result *result = argparse_parse(parser, argc, argv);
```

### Accessing Values

```c
const char *str = argparse_get_string(result, "name");
int num = argparse_get_int(result, "count");
bool flag = argparse_get_bool(result, "verbose");
```

### Cleanup

```c
argparse_result_free(result);
argparse_free(parser);
```

## API Modules

| Module | Header | Description |
|--------|--------|-------------|
| [Parser](parser.md) | `argparse_argument.h` | Parser lifecycle and configuration |
| [Options](options.md) | `argparse_argument.h` | Option creation and configuration |
| [Namespace](namespace.md) | `argparse_namespace.h` | Parsing and value access |
| [Groups](groups.md) | `argparse_group.h` | Argument grouping |
| [Mutex](mutex.md) | `argparse_mutex.h` | Mutually exclusive option groups |
| [Commands](commands.md) | `argparse_command.h` | Subcommands |
| [Validators](validators.md) | `argparse_validator.h` | Custom validation |
| [Validators Extra](validator-extra.md) | `argparse_validator_extra.h` | Built-in range/pattern validators |
| [Actions](actions.md) | `argparse_types.h` | Option actions |
| [Formatting](format.md) | `argparse_format.h` | Help output |
| [Memory](memory.md) | `argparse_memory.h` | Custom allocators |
| [Errors](errors.md) | `argparse_error.h` | Error handling |
| [Config](config.md) | `argparse_config.h` | Config files and env vars |
| [Completion](completion.md) | `argparse_complete.h` | Shell completion |
| [Version](version.md) | `argparse_version.h` | Version information |
| [Color](color.md) | `argparse_color.h` | Colored terminal output |
| [Thread](thread.md) | `argparse_thread.h` | Thread-safe parsing |
| [Batch](batch.md) | `argparse_batch.h` | Batch option registration |
| [Unicode](unicode.md) | `argparse_unicode.h` | UTF-8 string support |
| [Lazy Defaults](lazy.md) | `argparse_lazy.h` | On-demand default values |
| [I18n](i18n.md) | `argparse_i18n.h` | Internationalization |
| [Manpage](manpage.md) | `argparse_manpage.h` | Man page generation |
| [JSON](json.md) | `argparse_json.h` | JSON export |
| [Plugin](plugin.md) | `argparse_plugin.h` | Custom type plugins |
