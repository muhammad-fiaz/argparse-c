#include "argparse_internal.h"
#include "argparse-c/argparse_version.h"

const char *argparse_version_string(void)
{
    return ARGPARSE_VERSION_STRING;
}

void argparse_version(int *major, int *minor, int *patch)
{
    if (major) *major = ARGPARSE_VERSION_MAJOR;
    if (minor) *minor = ARGPARSE_VERSION_MINOR;
    if (patch) *patch = ARGPARSE_VERSION_PATCH;
}
