# Error Handling

Demonstrates comprehensive error handling with detailed diagnostics.

## Running

```bash
./error_handling --port abc              # Invalid value
./error_handling --name foo --name bar   # Too many values
./error_handling                         # Missing required option
./error_handling --unknown-flag          # Unknown option
./error_handling --verbose               # Missing value for --name
```

## What It Demonstrates

- Error code enumeration (`ARGPARSE_OK`, `ARGPARSE_ERROR_INVALID_VALUE`, etc.)
- `argparse_result_error()` for human-readable messages
- `argparse_result_error_code()` for programmatic handling
- Did-you-mean suggestions for misspelled options
- Required option validation

## Full Code

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(void) {
    struct argparse *parser = argparse_new("error_handling",
        "Demonstrate error handling");

    struct argparse_option *name_opt = argparse_add_option(
        parser, 'n', "name", ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
        "User name", "name");
    argparse_option_set_required(name_opt, true);

    argparse_add_option(parser, 'p', "port", ARGPARSE_NARGS_1,
        ARGPARSE_TYPE_INT, "Port number", "port");

    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
        ARGPARSE_TYPE_NONE, "Verbose", NULL);

    const char *argv[] = {"error_handling", "--port", "abc"};
    struct argparse_result *result = argparse_parse(parser, 3, argv);

    switch (argparse_result_error_code(result)) {
    case ARGPARSE_OK:
        printf("Parsed successfully\n");
        break;
    case ARGPARSE_ERROR_MISSING_ARGUMENT:
        fprintf(stderr, "Missing required argument: %s\n",
                argparse_result_error(result));
        break;
    case ARGPARSE_ERROR_INVALID_VALUE:
        fprintf(stderr, "Invalid value: %s\n",
                argparse_result_error(result));
        break;
    case ARGPARSE_ERROR_UNKNOWN:
        fprintf(stderr, "Unknown option: %s\n",
                argparse_result_error(result));
        break;
    case ARGPARSE_ERROR_AMBIGUOUS_OPTION:
        fprintf(stderr, "Ambiguous option: %s\n",
                argparse_result_error(result));
        break;
    default:
        fprintf(stderr, "Error (%d): %s\n",
                argparse_result_error_code(result),
                argparse_result_error(result));
        break;
    }

    printf("Error string: %s\n",
           argparse_error_string(argparse_result_error_code(result)));

    argparse_result_free(result);
    argparse_free(parser);
    return argparse_result_error_code(result) != ARGPARSE_OK ? 1 : 0;
}
```
