---
title: "JSON Export API Reference"
description: "JSON schema and value export without external dependencies. int argparse_export_json_schema(const struct argparse *parser, FILE *fp); Export the parser schema as JSON to a FILE stream. Returns 0 on success, -1 on failure. int argparse_export_json_schema_file(const struct argparse *parser, Export the parser schema as JSON to a file. Returns 0 on success, -1 on failure. int argparse_export_json_values(const struct argparse_result *result, Export parsed values as JSON to a FILE stream. Returns 0 on success, -1 on failure. int argparse_export_json_values_file(const struct argparse_result *result, Export parsed values as JSON to a file. Returns 0 on success, -1 on failure."
---

# JSON Export API Reference

JSON schema and value export without external dependencies.

## Functions

### `argparse_export_json_schema`

```c
int argparse_export_json_schema(const struct argparse *parser, FILE *fp);
```

Export the parser schema as JSON to a FILE stream. Returns 0 on success, -1 on failure.

### `argparse_export_json_schema_file`

```c
int argparse_export_json_schema_file(const struct argparse *parser,
                                     const char *filename);
```

Export the parser schema as JSON to a file. Returns 0 on success, -1 on failure.

### `argparse_export_json_values`

```c
int argparse_export_json_values(const struct argparse_result *result,
                                FILE *fp);
```

Export parsed values as JSON to a FILE stream. Returns 0 on success, -1 on failure.

### `argparse_export_json_values_file`

```c
int argparse_export_json_values_file(const struct argparse_result *result,
                                     const char *filename);
```

Export parsed values as JSON to a file. Returns 0 on success, -1 on failure.
