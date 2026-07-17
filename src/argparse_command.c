#include "argparse_internal.h"

#include <string.h>

struct argparse *argparse_new(const char *prog, const char *description);
void argparse_free(struct argparse *parser);

struct argparse_command *argparse_add_command(struct argparse *parser,
                                               const char *name,
                                               const char *help,
                                               argparse_callback_fn callback)
{
    if (!parser || !name) return NULL;

    const argparse_allocator *a = parser->allocator;

    if (parser->command_count >= parser->command_capacity) {
        size_t new_cap = parser->command_capacity == 0 ? 4 : parser->command_capacity * 2;
        struct argparse_command *new_cmds = (struct argparse_command *)argparse_realloc(
            parser->commands,
            parser->command_capacity * sizeof(struct argparse_command),
            new_cap * sizeof(struct argparse_command), a);
        if (!new_cmds) return NULL;
        parser->commands = new_cmds;
        parser->command_capacity = new_cap;
    }

    struct argparse_command *cmd = &parser->commands[parser->command_count];
    memset(cmd, 0, sizeof(*cmd));

    cmd->name = argparse_strdup(name, a);
    cmd->help = argparse_strdup(help, a);
    cmd->callback = callback;

    if (!cmd->name) {
        argparse_free_internal(cmd->name, a);
        argparse_free_internal(cmd->help, a);
        return NULL;
    }

    parser->command_count++;
    return cmd;
}

struct argparse *argparse_command_get_parser(struct argparse_command *command)
{
    if (!command) return NULL;

    if (!command->parser) {
        command->parser = argparse_new(command->name, NULL);
    }
    return command->parser;
}

struct argparse_command *argparse_find_command(struct argparse *parser, const char *name)
{
    if (!parser || !name) return NULL;

    for (size_t i = 0; i < parser->command_count; i++) {
        if (parser->commands[i].name &&
            argparse_streq(parser->commands[i].name, name)) {
            return &parser->commands[i];
        }
    }
    return NULL;
}
