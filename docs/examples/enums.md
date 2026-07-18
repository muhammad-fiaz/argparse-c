# Enums

Demonstrates using `ARGPARSE_TYPE_ENUM` with `argparse_option_set_choices()` to restrict option values to a predefined set.

## Running

```bash
./enums --color red
./enums --color blue
./enums --color yellow   # Error: invalid choice
./enums                  # Uses default "auto"
```

## What It Demonstrates

- Enum choices with `argparse_option_set_choices()`
- Default values for enums
- Multiple enum options (color, log level)
- Error messages for invalid choices
