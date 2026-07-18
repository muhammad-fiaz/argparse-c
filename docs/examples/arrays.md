---
title: "Arrays & Lists"
description: "Examples for multi-value arguments using STAR and PLUS nargs."
---

# Arrays & Lists

Multi-value arguments with nargs=STAR and nargs=PLUS.

## Zero or More Values

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("cat", "Concatenate files");

    argparse_add_option(parser, 'f', "files", ARGPARSE_NARGS_STAR,
                        ARGPARSE_TYPE_STRING, "Input files", "files");

    struct argparse_result *result = argparse_parse(parser, argc, argv);
    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    size_t count = argparse_get_count(result, "files");
    printf("Received %zu files:\n", count);

    for (size_t i = 0; i < count; i++) {
        const char *file = argparse_get_string_at(result, "files", i);
        printf("  %zu: %s\n", i + 1, file);
    }

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

```bash
$ ./cat -f file1.txt file2.txt file3.txt
Received 3 files:
  1: file1.txt
  2: file2.txt
  3: file3.txt
```

## One or More Values

```c
struct argparse *parser = argparse_new("compile", "Compile source files");

argparse_add_option(parser, 'I', "include", ARGPARSE_NARGS_PLUS,
                    ARGPARSE_TYPE_STRING, "Include paths", "includes");
argparse_add_positional(parser, ARGPARSE_NARGS_PLUS, ARGPARSE_TYPE_STRING,
                        "Source files", "sources");
```

```bash
$ ./compile -I /usr/include /usr/local/include src/main.c src/util.c
```

## Append Action

```c
struct argparse *parser = argparse_new("app", "Application");

argparse_add_option(parser, 'D', "define", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_STRING, "Define macro", "defines");
argparse_option_set_action(/* ... */, ARGPARSE_APPEND);
```

```bash
$ ./app -D DEBUG -D RELEASE -D VERSION=2
```

## Accessing Arrays

```c
size_t count = argparse_get_count(result, "files");
for (size_t i = 0; i < count; i++) {
    const char *val = argparse_get_string_at(result, "files", i);
    /* Process val */
}
```

## Mixed Positionals and Options

```c
struct argparse *parser = argparse_new("cp", "Copy files");

argparse_add_option(parser, 'r', "recursive", ARGPARSE_NARGS_0,
                    ARGPARSE_TYPE_NONE, "Recursive copy", "recursive");
argparse_add_positional(parser, ARGPARSE_NARGS_PLUS, ARGPARSE_TYPE_STRING,
                        "Source files", "sources");
argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                        "Destination", "dest");
```

```bash
$ cp -r src/ lib/ /backup/
```
