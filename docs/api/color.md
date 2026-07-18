# Color API Reference

Colored terminal output support for help and error messages.

## Functions

### `argparse_color_enable`

```c
void argparse_color_enable(bool enable);
```

Enable or disable colored output globally.

### `argparse_color_enabled`

```c
bool argparse_color_enabled(void);
```

Check if colored output is currently enabled.

### `argparse_color_code`

```c
const char *argparse_color_code(argparse_color color);
```

Get the ANSI escape sequence for a color. Returns an empty string if color is disabled or for `ARGPARSE_COLOR_DEFAULT`.

### `argparse_print_colored`

```c
void argparse_print_colored(FILE *fp, const char *text, argparse_color color);
```

Print colored text to a FILE stream. Automatically suppresses escape sequences when output is not a TTY.

## Enumerations

### `argparse_color`

| Value | Description |
|-------|-------------|
| `ARGPARSE_COLOR_DEFAULT` | Reset to default |
| `ARGPARSE_COLOR_RED` | Red text |
| `ARGPARSE_COLOR_GREEN` | Green text |
| `ARGPARSE_COLOR_YELLOW` | Yellow text |
| `ARGPARSE_COLOR_BLUE` | Blue text |
| `ARGPARSE_COLOR_MAGENTA` | Magenta text |
| `ARGPARSE_COLOR_CYAN` | Cyan text |
| `ARGPARSE_COLOR_BOLD` | Bold text |
