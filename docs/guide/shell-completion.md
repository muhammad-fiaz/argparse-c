---
title: "Shell Completion"
description: "Generate shell completion scripts for Bash, Zsh, Fish, and PowerShell."
---

# Shell Completion

argparse-c can generate shell completion scripts for Bash, Zsh, Fish, and PowerShell, enabling tab-completion for your CLI application.

## Overview

Shell completion scripts introspect your parser's options and subcommands to provide accurate, context-aware completions. Users can source the script in their shell configuration for automatic completions.

## Generating Completions

### Bash

```c
/* Add a --completions option */
argparse_add_option(parser, 0, "completions", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_STRING, "Generate shell completions",
                    "completions");

/* In your argument handling: */
const char *shell = argparse_get_string(result, "completions");
if (shell && strcmp(shell, "bash") == 0) {
    argparse_complete_bash(parser, argc, argv);
    return 0;
}
```

### Zsh

```c
if (shell && strcmp(shell, "zsh") == 0) {
    argparse_complete_zsh(parser, argc, argv);
    return 0;
}
```

### Fish

```c
if (shell && strcmp(shell, "fish") == 0) {
    argparse_complete_fish(parser, argc, argv);
    return 0;
}
```

### PowerShell

```c
if (shell && strcmp(shell, "powershell") == 0) {
    argparse_complete_powershell(parser, argc, argv);
    return 0;
}
```

## Installation

### Bash

Add to `~/.bashrc` or `~/.bash_profile`:

```bash
# For myapp
eval "$(myapp --completions bash)"
```

Or save to a file and source it:

```bash
myapp --completions bash > ~/.bash_completion.d/myapp
source ~/.bash_completion.d/myapp
```

### Zsh

Add to `~/.zshrc`:

```bash
# For myapp
eval "$(myapp --completions zsh)"
```

Or install to a completions directory:

```bash
myapp --completions zsh > ~/.zsh/completions/_myapp
```

### Fish

```fish
# Save to Fish completions directory
myapp --completions fish > ~/.config/fish/completions/myapp.fish
```

### PowerShell

```powershell
# Add to PowerShell profile
myapp --completions powershell | Out-String | Invoke-Expression
```

## What Gets Completed

The completion scripts automatically include:

- **Short options** (`-v`, `-o`)
- **Long options** (`--verbose`, `--output`)
- **Subcommands** (`init`, `build`, `deploy`)
- **Option descriptions** (in Zsh and PowerShell)
- **Type-aware completions** (e.g., file paths for file arguments)

## Complete Example

```c
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <string.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("git-like", "A git-like CLI");

    /* Add --completions option first */
    argparse_add_option(parser, 0, "completions", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Generate completions",
                        "completions");

    /* Add subcommands */
    struct argparse_command *init_cmd =
        argparse_add_command(parser, "init", "Initialize repository", NULL);
    struct argparse *init_parser = argparse_command_get_parser(init_cmd);
    argparse_add_option(init_parser, 'f', "force", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Force init", NULL);

    struct argparse_command *commit_cmd =
        argparse_add_command(parser, "commit", "Commit changes", NULL);
    struct argparse *commit_parser = argparse_command_get_parser(commit_cmd);
    argparse_add_option(commit_parser, 'm', "message", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Commit message", "message");

    /* Check for completion request before full parsing */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--completions") == 0 && i + 1 < argc) {
            const char *shell = argv[i + 1];
            if (strcmp(shell, "bash") == 0) {
                argparse_complete_bash(parser, argc, argv);
            } else if (strcmp(shell, "zsh") == 0) {
                argparse_complete_zsh(parser, argc, argv);
            } else if (strcmp(shell, "fish") == 0) {
                argparse_complete_fish(parser, argc, argv);
            } else if (strcmp(shell, "powershell") == 0) {
                argparse_complete_powershell(parser, argc, argv);
            }
            argparse_free(parser);
            return 0;
        }
    }

    /* Normal argument parsing */
    struct argparse_result *result = argparse_parse(parser, argc, argv);
    /* ... handle result ... */

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```
