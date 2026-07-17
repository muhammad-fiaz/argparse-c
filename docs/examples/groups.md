# Groups

Organizing options into logical groups for help output.

## Complete Example

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("server", "HTTP server");

    /* General options (default group) */
    argparse_add_option(parser, 'h', "help", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Show help", NULL);

    /* Network options */
    struct argparse_group *network = argparse_add_group(parser,
        "Network Options", "Configure network settings");

    struct argparse_option *host_opt = argparse_add_option(parser,
        'H', "host", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Bind address", "host");
    argparse_group_add_option(network, host_opt);

    struct argparse_option *port_opt = argparse_add_option(parser,
        'p', "port", ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT,
        "Listen port", "port");
    argparse_group_add_option(network, port_opt);

    /* Security options */
    struct argparse_group *security = argparse_add_group(parser,
        "Security Options", "Configure security settings");

    struct argparse_option *ssl_opt = argparse_add_option(parser,
        's', "ssl", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE,
        "Enable SSL/TLS", "ssl");
    argparse_group_add_option(security, ssl_opt);

    struct argparse_option *cert_opt = argparse_add_option(parser,
        'c', "cert", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "SSL certificate file", "cert");
    argparse_group_add_option(security, cert_opt);

    struct argparse_option *key_opt = argparse_add_option(parser,
        'k', "key", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "SSL key file", "key");
    argparse_group_add_option(security, key_opt);

    /* Logging options */
    struct argparse_group *logging = argparse_add_group(parser,
        "Logging Options", "Configure logging");

    struct argparse_option *log_level_opt = argparse_add_option(parser,
        'l', "log-level", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Log level (debug, info, warn, error)", "log_level");
    argparse_group_add_option(logging, log_level_opt);

    struct argparse_option *log_file_opt = argparse_add_option(parser,
        'f', "log-file", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "Log output file", "log_file");
    argparse_group_add_option(logging, log_file_opt);

    struct argparse_result *result = argparse_parse(parser, argc, argv);
    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    printf("Host: %s\n", argparse_get_string(result, "host"));
    printf("Port: %d\n", argparse_get_int(result, "port"));
    printf("SSL: %s\n", argparse_get_bool(result, "ssl") ? "enabled" : "disabled");
    printf("Log level: %s\n", argparse_get_string(result, "log_level"));

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Help Output

```
usage: server [-h] [-H HOST] [-p PORT] [-s] [-c CERT] [-k KEY]
              [-l LOG_LEVEL] [-f LOG_FILE]

HTTP server

Network Options:
  Configure network settings
  -H, --host HOST          Bind address
  -p, --port PORT          Listen port

Security Options:
  Configure security settings
  -s, --ssl                Enable SSL/TLS
  -c, --cert CERT          SSL certificate file
  -k, --key KEY            SSL key file

Logging Options:
  Configure logging
  -l, --log-level LEVEL    Log level (debug, info, warn, error)
  -f, --log-file FILE      Log output file

options:
  -h, --help               Show this help message and exit
```

## Key Points

- Groups organize options in help output
- Options not in any group appear in the default "options" section
- Groups are displayed in creation order
- Useful for complex CLIs with many options
