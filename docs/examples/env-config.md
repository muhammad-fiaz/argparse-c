# Environment & Config

Demonstrates reading option values from environment variables and config files.

## Running

```bash
MYAPP_OUTPUT_DIR=./out MYAPP_VERBOSE=1 ./env_config
./env_config --output-dir /custom/path
MYAPP_COLOR=blue ./env_config
```

## What It Demonstrates

- `argparse_set_env_prefix()` for automatic env var lookup
- `argparse_option_set_env()` for explicit env var mapping
- Priority: CLI args > env vars > defaults
- Config file loading patterns
