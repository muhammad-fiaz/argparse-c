# Advanced Features

Demonstrates callbacks, store_const, required options, and combining multiple advanced features.

## Running

```bash
./advanced --log
./advanced --log-level debug
./advanced --config /etc/myapp.conf
./advanced --dry-run --verbose
```

## What It Demonstrates

- Option callbacks with `argparse_option_set_callback()`
- Store const with `ARGPARSE_STORE_CONST` action
- Required options with `argparse_option_set_required()`
- Combining multiple features in a realistic CLI
