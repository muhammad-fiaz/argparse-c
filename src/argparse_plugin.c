#include "argparse-c/argparse_plugin.h"
#include "argparse_internal.h"

#include <string.h>

#define argparse_MAX_CUSTOM_TYPES 32

typedef struct argparse_custom_type_entry {
    char *type_name;
    argparse_type_parse_fn parse_fn;
    bool in_use;
} argparse_custom_type_entry;

static argparse_custom_type_entry custom_types[argparse_MAX_CUSTOM_TYPES];
static bool plugin_initialized = false;

static void ensure_initialized(void)
{
    if (!plugin_initialized) {
        memset(custom_types, 0, sizeof(custom_types));
        plugin_initialized = true;
    }
}

int argparse_register_type(const char *type_name,
                            argparse_type_parse_fn parse_fn)
{
    if (!type_name || !parse_fn) return -1;

    ensure_initialized();

    for (int i = 0; i < argparse_MAX_CUSTOM_TYPES; i++) {
        if (custom_types[i].in_use &&
            strcmp(custom_types[i].type_name, type_name) == 0) {
            custom_types[i].parse_fn = parse_fn;
            return 0;
        }
    }

    for (int i = 0; i < argparse_MAX_CUSTOM_TYPES; i++) {
        if (!custom_types[i].in_use) {
            custom_types[i].type_name = argparse_strdup(type_name, NULL);
            if (!custom_types[i].type_name) return -1;
            custom_types[i].parse_fn = parse_fn;
            custom_types[i].in_use = true;
            return 0;
        }
    }

    return -1;
}

void argparse_unregister_type(const char *type_name)
{
    if (!type_name) return;

    ensure_initialized();

    for (int i = 0; i < argparse_MAX_CUSTOM_TYPES; i++) {
        if (custom_types[i].in_use &&
            strcmp(custom_types[i].type_name, type_name) == 0) {
            argparse_free_internal(custom_types[i].type_name, NULL);
            custom_types[i].type_name = NULL;
            custom_types[i].parse_fn = NULL;
            custom_types[i].in_use = false;
            return;
        }
    }
}
