# Options & Types

Demonstrates all option types with custom defaults.

## Running

```bash
./options --name Alice --port 8080 --rate 3.14 --color blue
./options                          # Uses defaults
./options --help                   # Show all options
```

## What It Demonstrates

- **STRING** option with default value
- **INT** option with default value
- **UINT** option for unsigned integers
- **FLOAT** and **DOUBLE** options
- **BOOL** option
- **ENUM** option with `argparse_option_set_choices()`
- Setting defaults with `argparse_option_set_default()`
