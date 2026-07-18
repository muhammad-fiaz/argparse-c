# Unicode API Reference

UTF-8 string support with proper character width detection.

## Functions

### `argparse_utf8_strlen`

```c
size_t argp_utf8_strlen(const char *str);
```

Get the number of Unicode codepoints in a UTF-8 string. Returns the count of visible characters, properly handling multi-byte sequences.

## Notes

- All string handling in argparse-c is UTF-8 aware
- Help text formatting accounts for wide characters (CJK, emoji) when computing alignment
- String length calculations use codepoint count, not byte count
- The library does not validate UTF-8 sequences; invalid sequences are treated as individual bytes
