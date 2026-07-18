# Error Handling

Demonstrates comprehensive error handling with detailed diagnostics.

## Running

```bash
./error_handling --port abc              # Invalid value
./error_handling --port 80 --port 443    # Too many values
./error_handling                         # Missing required option
./error_handling --unknown-flag          # Unknown option
```

## What It Demonstrates

- Error code enumeration (`ARGPARSE_OK`, `ARGPARSE_ERROR_INVALID_VALUE`, etc.)
- `argparse_result_error()` for human-readable messages
- `argparse_result_error_code()` for programmatic handling
- Did-you-mean suggestions for misspelled options
- Required option validation
