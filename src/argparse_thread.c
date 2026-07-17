#include "argparse_internal.h"
#include "argparse-c/argparse_thread.h"

struct argparse_result *argparse_parse(struct argparse *parser,
                                       int argc, const char **argv);

void argparse_freeze(struct argparse *parser)
{
    if (!parser) return;
    parser->frozen = true;
}

bool argparse_is_frozen(const struct argparse *parser)
{
    if (!parser) return false;
    return parser->frozen;
}

struct argparse_result *argparse_parse_threadsafe(struct argparse *parser,
                                                   int argc,
                                                   const char **argv)
{
    if (!parser || !parser->frozen) return NULL;
    return argparse_parse(parser, argc, argv);
}
