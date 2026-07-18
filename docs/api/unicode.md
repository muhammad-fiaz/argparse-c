# Unicode API Reference

UTF-8 string support with proper character width detection.

## Functions

### `argparse_utf8_strlen`

```c
size_t argparse_utf8_strlen(const char *str);
```

Get the display width of a UTF-8 string in terminal columns. Accounts for wide characters (CJK, emoji) and zero-width characters (combining marks).

**Parameters:**
- `str` — The UTF-8 string to measure. Must not be NULL.

**Returns:** The display width in columns.

## Notes

- All string handling in argparse-c is UTF-8 aware
- Help text formatting accounts for wide characters (CJK, emoji) when computing alignment
- Display width is calculated per Unicode East Asian Width and zero-width semantics
- The library does not validate UTF-8 sequences; invalid sequences are treated as individual bytes
- Each invalid byte counts as one column width

## Example

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(void) {
    printf("ASCII: %zu\n", argparse_utf8_strlen("hello"));     /* 5 */
    printf("Accent: %zu\n", argparse_utf8_strlen("\xC3\xA9"));  /* 1 (é) */
    printf("CJK: %zu\n", argparse_utf8_strlen("\xE4\xB8\xAD")); /* 1 (中) */
    return 0;
}
```
