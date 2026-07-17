# Security Policy

## Supported Versions

| Version | Supported          |
|---------|--------------------|
| 0.1.x   | :white_check_mark: |
| < 0.1   | :x:                |

## Reporting a Vulnerability

If you discover a security vulnerability within argparse-c, please send an email to [contact@muhammadfiaz.com](mailto:contact@muhammadfiaz.com). All security vulnerabilities will be promptly addressed.

**Please do NOT report security vulnerabilities through public GitHub issues.**

### What to include

When reporting a vulnerability, please include:

- Description of the vulnerability
- Steps to reproduce the issue
- Potential impact
- Suggested fix (if any)

### Response timeline

- **Acknowledgment**: Within 48 hours
- **Initial assessment**: Within 1 week
- **Fix release**: Depends on severity, typically within 2 weeks

## Security Best Practices

When using argparse-c in your project:

1. **Always validate user input** — use validators for custom constraints
2. **Use safe defaults** — set appropriate default values for options
3. **Limit input lengths** — consider adding validators for maximum string lengths
4. **Keep the library updated** — upgrade to the latest version for security patches

## Memory Safety

argparse-c is written in C99 and takes memory safety seriously:

- All public API functions handle NULL inputs gracefully
- Custom allocator support for embedded environments
- No buffer overflows by design (use of safe string operations)
- Deterministic cleanup via `argparse_free()` and `argparse_result_free()`

For additional assurance, enable sanitizers during testing:

```bash
cmake -B build \
  -DCMAKE_C_FLAGS="-fsanitize=address,undefined" \
  -DARGPARSE_BUILD_TESTS=ON
```

## Acknowledgments

We appreciate the security research community and responsibly disclose vulnerabilities.
