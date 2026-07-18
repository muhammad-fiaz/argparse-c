# Colored Output

argparse-c supports colored terminal output for help messages and error diagnostics.

## Enabling Color

```c
#include <argparse-c/argparse.h>

// Enable colored output globally
argparse_color_enable(true);

// Disable it (e.g., for piped output)
argparse_color_enable(false);
```

Color output is enabled by default when the output stream is a TTY.

## Available Colors

| Color | Constant | Description |
|-------|----------|-------------|
| Red | `ARGPARSE_COLOR_RED` | Error messages |
| Green | `ARGPARSE_COLOR_GREEN` | Success indicators |
| Yellow | `ARGPARSE_COLOR_YELLOW` | Warnings |
| Blue | `ARGPARSE_COLOR_BLUE` | Information |
| Magenta | `ARGPARSE_COLOR_MAGENTA` | Highlights |
| Cyan | `ARGPARSE_COLOR_CYAN` | Secondary text |
| Bold | `ARGPARSE_COLOR_BOLD` | Bold emphasis |
| Default | `ARGPARSE_COLOR_DEFAULT` | Reset to default |

## Printing Colored Text

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(void) {
    argparse_color_enable(true);

    argparse_print_colored(stderr, "Error: ", ARGPARSE_COLOR_RED);
    argparse_print_colored(stderr, "file not found\n", ARGPARSE_COLOR_DEFAULT);

    argparse_print_colored(stdout, "Warning: ", ARGPARSE_COLOR_YELLOW);
    argparse_print_colored(stdout, "deprecated option\n", ARGPARSE_COLOR_DEFAULT);

    return 0;
}
```

## Getting ANSI Codes Directly

```c
const char *code = argparse_color_code(ARGPARSE_COLOR_RED);
printf("%sRed text%s\n", code, argparse_color_code(ARGPARSE_COLOR_DEFAULT));
```

Returns an empty string when color is disabled or for `ARGPARSE_COLOR_DEFAULT`.

## TTY Detection

`argparse_print_colored()` automatically checks if the output stream is a TTY. When output is piped or redirected to a file, escape sequences are suppressed automatically. You can also explicitly disable color with `argparse_color_enable(false)`.
