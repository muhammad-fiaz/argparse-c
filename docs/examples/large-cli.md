# Large CLI Apps

Real-world project structure for complex command-line applications.

## Project Structure

```
myapp/
├── CMakeLists.txt
├── include/
│   └── myapp/
│       ├── app.h
│       ├── commands.h
│       └── config.h
├── src/
│   ├── main.c
│   ├── app.c
│   ├── commands/
│   │   ├── init.c
│   │   ├── build.c
│   │   └── deploy.c
│   └── config.c
└── tests/
    └── test_main.c
```

## Main Entry Point

```c
/* src/main.c */
#include <argparse-c/argparse.h>
#include <stdio.h>

/* Forward declarations */
void register_init(struct argparse *parser);
void register_build(struct argparse *parser);
void register_deploy(struct argparse *parser);

int main(int argc, const char **argv) {
    struct argparse *parser = argparse_new("mytool",
        "A comprehensive project management tool");

    argparse_set_epilog(parser,
        "Examples:\n"
        "  mytool init --name myproject\n"
        "  mytool build --target release\n"
        "  mytool deploy --env production");

    /* Register global options */
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose output", "verbose");
    argparse_add_option(parser, 'q', "quiet", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Quiet mode", "quiet");
    argparse_add_option(parser, 0, "config", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Config file", "config");

    /* Register commands */
    register_init(parser);
    register_build(parser);
    register_deploy(parser);

    /* Parse */
    struct argparse_result *result = argparse_parse(parser, argc, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        fprintf(stderr, "Error: %s\n", argparse_result_error(result));
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
```

## Command Registration

```c
/* src/commands/init.c */
#include <argparse-c/argparse.h>
#include <stdio.h>

static void init_callback(struct argparse *parser, void *data) {
    struct argparse_result *result = (struct argparse_result *)data;
    const char *name = argparse_get_string(result, "name");
    const char *template = argparse_get_string(result, "template");
    bool force = argparse_get_bool(result, "force");

    printf("Initializing project: %s\n", name ? name : ".");
    if (template) printf("  Template: %s\n", template);
    if (force) printf("  (forced)\n");
}

void register_init(struct argparse *parser) {
    struct argparse_command *cmd = argparse_add_command(
        parser, "init", "Initialize a new project", init_callback);
    struct argparse *sub = argparse_command_get_parser(cmd);

    argparse_add_option(sub, 'n', "name", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Project name", "name");
    argparse_add_option(sub, 't', "template", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Project template", "template");
    argparse_add_option(sub, 'f', "force", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Force initialization", "force");
}
```

```c
/* src/commands/build.c */
#include <argparse-c/argparse.h>
#include <stdio.h>

static void build_callback(struct argparse *parser, void *data) {
    struct argparse_result *result = (struct argparse_result *)data;
    const char *target = argparse_get_string(result, "target");
    bool release = argparse_get_bool(result, "release");
    int jobs = argparse_get_int(result, "jobs");

    printf("Building project...\n");
    printf("  Target: %s\n", target ? target : "default");
    printf("  Mode: %s\n", release ? "release" : "debug");
    if (jobs > 0) printf("  Jobs: %d\n", jobs);
}

void register_build(struct argparse *parser) {
    struct argparse_command *cmd = argparse_add_command(
        parser, "build", "Build the project", build_callback);
    struct argparse *sub = argparse_command_get_parser(cmd);

    argparse_add_option(sub, 't', "target", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Build target", "target");
    argparse_add_option(sub, 'r', "release", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Release build", "release");
    argparse_add_option(sub, 'j', "jobs", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Parallel jobs", "jobs");
}
```

## Best Practices

1. **Modular commands**: Each command in its own file
2. **Shared options**: Use a helper function for common options
3. **Config loading**: Load config before parsing CLI args
4. **Error handling**: Consistent error reporting across commands
5. **Help organization**: Use groups for complex option sets
6. **Testing**: Test each command independently

## CMake Integration

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(mytool VERSION 1.0.0 LANGUAGES C)

add_executable(mytool
    src/main.c
    src/commands/init.c
    src/commands/build.c
    src/commands/deploy.c
)

target_include_directories(mytool PRIVATE include)
target_link_libraries(mytool PRIVATE argparse-c-static)

# Install
install(TARGETS mytool DESTINATION bin)
```
