---
title: "Extra Validators API Reference"
description: "Built-in validators for range checking and pattern matching. bool argparse_validate_int_range(const char *value, char *error_buf, Range validator for integers. Checks that the parsed integer value is within the range configured on the option via `argparse_option_set_range_int()`. Returns true if valid. bool argparse_validate_float_range(const char *value, char *error_buf, Range validator for floats. Checks that the parsed float value is within the range configured on the option via `argparse_option_set_range_float()`. Returns true if valid. bool argparse_validate_pattern(const char *value, char *error_buf, Pattern validator using glob-style matching. Supports `?` (any single character) and `*` (any sequence). The pattern is read from the option's pattern field set via `argparse_option_set_pattern()`. Returns true if valid. void argparse_option_set_range_int(struct argparse_option *option, Configure integer range constraints and attach the range validator. void argparse_option_set_range_float(struct argparse_option *option, Configure float range constraints and attach the range validator. void argparse_option_set_pattern(struct argparse_option *option, Configure a glob pattern constraint and attach the pattern validator. bool argparse_validate_nonempty(const char *value, char *error_buf, Validates that a string is not empty. Returns true if the string has content."
---

# Extra Validators API Reference

Built-in validators for range checking and pattern matching.

## Range Validators

### `argparse_validate_int_range`

```c
bool argparse_validate_int_range(const char *value, char *error_buf,
                                 size_t error_buf_size);
```

Range validator for integers. Checks that the parsed integer value is within the range configured on the option via `argparse_option_set_range_int()`. Returns true if valid.

### `argparse_validate_float_range`

```c
bool argparse_validate_float_range(const char *value, char *error_buf,
                                   size_t error_buf_size);
```

Range validator for floats. Checks that the parsed float value is within the range configured on the option via `argparse_option_set_range_float()`. Returns true if valid.

## Pattern Validator

### `argparse_validate_pattern`

```c
bool argparse_validate_pattern(const char *value, char *error_buf,
                               size_t error_buf_size);
```

Pattern validator using glob-style matching. Supports `?` (any single character) and `*` (any sequence). The pattern is read from the option's pattern field set via `argparse_option_set_pattern()`. Returns true if valid.

## Convenience Configuration Functions

### `argparse_option_set_range_int`

```c
void argparse_option_set_range_int(struct argparse_option *option,
                                   int min_val, int max_val);
```

Configure integer range constraints and attach the range validator.

### `argparse_option_set_range_float`

```c
void argparse_option_set_range_float(struct argparse_option *option,
                                     float min_val, float max_val);
```

Configure float range constraints and attach the range validator.

### `argparse_option_set_pattern`

```c
void argparse_option_set_pattern(struct argparse_option *option,
                                 const char *pattern);
```

Configure a glob pattern constraint and attach the pattern validator.

## Non-empty Validator

### `argparse_validate_nonempty`

```c
bool argparse_validate_nonempty(const char *value, char *error_buf,
                                size_t error_buf_size);
```

Validates that a string is not empty. Returns true if the string has content.
