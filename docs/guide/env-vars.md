---
title: Environment Variables
description: Using environment variables with arguments
---

# Environment Variables

argparse-c can read default values from environment variables.

## Basic Usage

```c
argparse_add_option(parser, 'p', "port", ARGPARSE_NARGS_1,
                    ARGPARSE_TYPE_INT, "Server port", "PORT");

/* Check environment variable as fallback */
int port = argparse_get_int(result, "port");
if (port == 0) {
    const char *env_port = getenv("MYAPP_PORT");
    if (env_port) {
        port = atoi(env_port);
    } else {
        port = 8080; /* Default */
    }
}
```

## Environment Variable Pattern

### Define Constants

```c
#define ENV_PREFIX "MYAPP_"
#define ENV_PORT   ENV_PREFIX "PORT"
#define ENV_HOST   ENV_PREFIX "HOST"
#define ENV_VERBOSE ENV_PREFIX "VERBOSE"
```

### Apply Defaults

```c
int apply_env_defaults(struct argparse_result *result) {
    /* Port */
    if (argparse_get_int(result, "port") == 0) {
        const char *env = getenv(ENV_PORT);
        if (env) {
            /* Set default from environment */
        }
    }

    /* Host */
    if (!argparse_get_string(result, "host")) {
        const char *env = getenv(ENV_HOST);
        if (env) {
            /* Set default from environment */
        }
    }

    return 0;
}
```

## Example: Environment-Aware CLI

```c
#include <argparse-c/argparse.h>
#include <stdio.h>
#include <stdlib.h>

#define APP_ENV_PREFIX "MYTOOL_"

static const char *get_env_or_default(const char *env_name, const char *def) {
    const char *val = getenv(env_name);
    return val ? val : def;
}

int main(int argc, char *argv[]) {
    struct argparse *parser = argparse_new("mytool", "A configurable tool");

    argparse_add_option(parser, 'p', "port", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Server port", "PORT");
    argparse_add_option(parser, 'H', "host", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Server host", "HOST");
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose output", NULL);

    struct argparse_result *result = argparse_parse(
        parser, argc, (const char **)argv
    );

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    if (argparse_result_should_exit(result)) {
        argparse_result_free(result);
        argparse_free(parser);
        return 0;
    }

    /* Apply environment variable defaults */
    int port = argparse_get_int(result, "port");
    const char *host = argparse_get_string(result, "host");
    int verbose = argparse_get_bool(result, "verbose");

    if (port == 0) {
        const char *env_port = getenv(APP_ENV_PREFIX "PORT");
        port = env_port ? atoi(env_port) : 8080;
    }

    if (!host) {
        host = get_env_or_default(APP_ENV_PREFIX "HOST", "localhost");
    }

    if (!verbose) {
        const char *env_verbose = getenv(APP_ENV_PREFIX "VERBOSE");
        verbose = env_verbose && atoi(env_verbose);
    }

    printf("Server: %s:%d\n", host, port);
    printf("Verbose: %s\n", verbose ? "yes" : "no");

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Environment Variable Naming

Follow these conventions:

| Convention | Example | Usage |
|------------|---------|-------|
| `APP_OPTION` | `MYAPP_PORT` | Unix/Linux/macOS |
| `APP_OPTION` | `MYAPP_PORT` | Windows (recommended) |

## Best Practices

1. **Document env vars** - List them in help text
2. **Use prefix** - Avoid namespace collisions
3. **CLI overrides env** - Command-line takes precedence
4. **Validate env values** - Same validation as CLI arguments
5. **Provide defaults** - Don't require env vars

## Next Steps

- [Config Files](./config-files) - Loading from config files
- [API: Config](../api/config) - Complete API reference
