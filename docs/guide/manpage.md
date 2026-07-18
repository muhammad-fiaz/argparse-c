---
title: "Man Page Generation"
description: "argparse-c can generate Unix manual pages in groff/troff format directly from a parser definition. The output follows standard man page conventions with proper sections. int main(void) { man ./myapp.1 The man page includes these standard sections: Both functions return `0` on success, `-1` on failure."
---

# Man Page Generation

argparse-c can generate Unix manual pages in groff/troff format directly from a parser definition. The output follows standard man page conventions with proper sections.

## Quick Start

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(void) {
    struct argparse *parser = argparse_new("myapp", "My application");

    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Enable verbose output", NULL);
    argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Output file", "output");
    argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                            "Input file", "input");

    // Generate to stdout (pipe to man)
    argparse_generate_manpage(parser, stdout);

    // Or save to a file
    argparse_generate_manpage_file(parser, "myapp.1");

    argparse_free(parser);
    return 0;
}
```

## Viewing Generated Man Pages

```bash
# Generate and view directly
./myapp --help | groff -man -Tutf8 | less

# Save and view
./myapp > myapp.1
man ./myapp.1
```

## Generated Sections

The man page includes these standard sections:

| Section | Content |
|---------|---------|
| **NAME** | Program name and short description |
| **SYNOPSIS** | Usage summary with all options and positionals |
| **DESCRIPTION** | Full program description |
| **OPTIONS** | Detailed list of all options with flags, types, and help text |
| **EXAMPLES** | Placeholder for usage examples |
| **SEE ALSO** | Cross-references |
| **AUTHORS** | Program author |

## API Reference

| Function | Description |
|----------|-------------|
| `argparse_generate_manpage(parser, fp)` | Write man page to a FILE stream |
| `argparse_generate_manpage_file(parser, filename)` | Write man page to a file |

Both functions return `0` on success, `-1` on failure.
