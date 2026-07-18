---
title: "Completion API"
description: "Shell completion script generation for Bash, Zsh, Fish, and PowerShell."
---

# Completion API

Shell completion script generation.

## Functions

### `argparse_complete_bash`

```c
void argparse_complete_bash(struct argparse *parser,
                            int argc,
                            const char **argv);
```

Generate a Bash completion script to stdout.

### `argparse_complete_zsh`

```c
void argparse_complete_zsh(struct argparse *parser,
                           int argc,
                           const char **argv);
```

Generate a Zsh completion script to stdout.

### `argparse_complete_fish`

```c
void argparse_complete_fish(struct argparse *parser,
                            int argc,
                            const char **argv);
```

Generate a Fish completion script to stdout.

### `argparse_complete_powershell`

```c
void argparse_complete_powershell(struct argparse *parser,
                                  int argc,
                                  const char **argv);
```

Generate a PowerShell completion script to stdout.

## Example

```c
#include <argparse-c/argparse.h>
#include <string.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("myapp", "Application");

    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose output", NULL);
    argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Output file", "output");

    /* Check for completion request */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--completions") == 0 && i + 1 < argc) {
            const char *shell = argv[i + 1];
            if (strcmp(shell, "bash") == 0)
                argparse_complete_bash(parser, argc, argv);
            else if (strcmp(shell, "zsh") == 0)
                argparse_complete_zsh(parser, argc, argv);
            else if (strcmp(shell, "fish") == 0)
                argparse_complete_fish(parser, argc, argv);
            else if (strcmp(shell, "powershell") == 0)
                argparse_complete_powershell(parser, argc, argv);
            argparse_free(parser);
            return 0;
        }
    }

    /* Normal parsing... */
    struct argparse_result *result = argparse_parse(parser, argc, argv);
    /* ... */

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Installation

### Bash

```bash
# Add to ~/.bashrc
eval "$(myapp --completions bash)"
```

### Zsh

```bash
# Add to ~/.zshrc
eval "$(myapp --completions zsh)"
```

### Fish

```fish
# Save to ~/.config/fish/completions/myapp.fish
myapp --completions fish > ~/.config/fish/completions/myapp.fish
```

### PowerShell

```powershell
# Add to PowerShell profile
myapp --completions powershell | Out-String | Invoke-Expression
```

## Notes

- Completion scripts introspect the parser's options and subcommands
- Short and long options are both completed
- Subcommand-aware completions are provided
- Option descriptions are included in Zsh and PowerShell
