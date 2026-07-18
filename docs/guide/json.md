---
title: "JSON Export"
description: "Export parser schemas and parsed values as JSON."
---

# JSON Export

argparse-c can export parser schemas and parsed values as JSON without any external JSON library dependency.

## Exporting Parser Schema

The schema describes the parser's configuration including all options, their types, defaults, and constraints.

```c
#include <argparse-c/argparse.h>
#include <stdio.h>

int main(void) {
    struct argparse *parser = argparse_new("myapp", "My application");

    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose output", NULL);
    argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Output file", "output");
    argparse_add_option(parser, 'n', "count", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Count", "count");

    // Export schema to stdout
    argparse_export_json_schema(parser, stdout);

    // Or export to a file
    argparse_export_json_schema_file(parser, "schema.json");

    argparse_free(parser);
    return 0;
}
```

### Schema Output Format

```json
{
  "name": "myapp",
  "description": "My application",
  "options": [
    {
      "name": "verbose",
      "short": "v",
      "type": "none",
      "nargs": 0,
      "help": "Verbose output",
      "required": false,
      "dest": "verbose"
    },
    {
      "name": "output",
      "short": "o",
      "type": "string",
      "nargs": 1,
      "help": "Output file",
      "required": false,
      "default": null,
      "dest": "output"
    }
  ]
}
```

## Exporting Parsed Values

After parsing, you can export the results as JSON:

```c
struct argparse_result *result = argparse_parse(parser, argc, argv);

// Export values to stdout
argparse_export_json_values(result, stdout);

// Or export to a file
argparse_export_json_values_file(result, "result.json");

argparse_result_free(result);
```

### Values Output Format

```json
{
  "verbose": true,
  "output": "file.txt",
  "count": 42
}
```

## API Reference

| Function | Description |
|----------|-------------|
| `argparse_export_json_schema(parser, fp)` | Write schema JSON to a FILE stream |
| `argparse_export_json_schema_file(parser, filename)` | Write schema JSON to a file |
| `argparse_export_json_values(result, fp)` | Write parsed values JSON to a FILE stream |
| `argparse_export_json_values_file(result, filename)` | Write parsed values JSON to a file |

All functions return `0` on success, `-1` on failure.
