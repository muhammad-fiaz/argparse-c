---
title: "Unicode Support"
description: "UTF-8 and Unicode support for arguments, help text, and values."
---

# Unicode Support

argparse-c provides full UTF-8 support for argument names, descriptions, help text, and values.

## Overview

All strings in argparse-c are expected to be UTF-8 encoded. The library correctly handles:

- Multi-byte UTF-8 characters in option names and descriptions
- Wide characters (CJK) for proper alignment in help output
- Zero-width characters that don't contribute to display width

## Display Width Calculation

The `argparse_utf8_strlen` function calculates the display width of a UTF-8 string in terminal columns:

```c
#include <argparse-c/argparse.h>

size_t width = argparse_utf8_strlen("Hello, 世界");
/* Returns 14 (5 ASCII + 1 comma + 1 space + 4 CJK columns) */
```

### How It Works

- ASCII characters count as 1 column
- CJK (Chinese, Japanese, Korean) characters count as 2 columns
- Combining characters (accents, diacritics) count as 0 columns
- Zero-width characters count as 0 columns

## Unicode in Help Text

```c
struct argparse *parser = argparse_new("unicode-app", "支持Unicode的应用程序");

argparse_add_option(parser, 'n', "name", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_STRING, "你的名字", "name");
argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_STRING, "输出文件", "output");

argparse_print_help(parser);
```

Output:

```
usage: unicode-app [-h] [-n NAME] [-o OUTPUT]

支持Unicode的应用程序

options:
  -n, --name NAME    你的名字
  -o, --output OUTPUT 输出文件
```

## Unicode in Values

argparse-c preserves UTF-8 values exactly as provided:

```c
struct argparse *parser = argparse_new("test", NULL);
argparse_add_option(parser, 'n', "name", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_STRING, NULL, "name");

const char *argv[] = {"test", "--name", "José García"};
struct argparse_result *result = argparse_parse(parser, 3, argv);

const char *name = argparse_get_string(result, "name");
/* name points to "José García" — UTF-8 preserved */
```

## Cross-Platform Notes

### Windows

On Windows, argparse-c uses UTF-8 throughout. For proper display in Windows Terminal, ensure:

- The application is built with UTF-8 support
- The terminal uses a UTF-8 codepage (`chcp 65001`)

### macOS/Linux

These platforms natively support UTF-8. No special configuration is needed.

## String Validation

argparse-c does not validate UTF-8 encoding of input strings. If you need to validate UTF-8, use a third-party library or implement your own validation before passing strings to the parser.
