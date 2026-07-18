---
title: "Man Page Generation API Reference"
description: "Groff/troff man page generation from parser definitions. int argparse_generate_manpage(const struct argparse *parser, FILE *fp); Generate a man page in groff/troff format to a FILE stream. Returns 0 on success, -1 on failure. Generated sections: NAME, SYNOPSIS, DESCRIPTION, OPTIONS, EXAMPLES, SEE ALSO, AUTHORS. int argparse_generate_manpage_file(const struct argparse *parser, Generate a man page and write it to a file. Returns 0 on success, -1 on failure. The generated file can be viewed with `man ./filename.1`."
---

# Man Page Generation API Reference

Groff/troff man page generation from parser definitions.

## Functions

### `argparse_generate_manpage`

```c
int argparse_generate_manpage(const struct argparse *parser, FILE *fp);
```

Generate a man page in groff/troff format to a FILE stream. Returns 0 on success, -1 on failure.

Generated sections: NAME, SYNOPSIS, DESCRIPTION, OPTIONS, EXAMPLES, SEE ALSO, AUTHORS.

### `argparse_generate_manpage_file`

```c
int argparse_generate_manpage_file(const struct argparse *parser,
                                   const char *filename);
```

Generate a man page and write it to a file. Returns 0 on success, -1 on failure.

The generated file can be viewed with `man ./filename.1`.
