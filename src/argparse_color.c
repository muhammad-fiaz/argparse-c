#include "argparse-c/argparse_color.h"

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#ifndef isatty
#define isatty _isatty
#endif
#ifndef fileno
#define fileno _fileno
#endif
#else
#include <unistd.h>
#endif

static bool color_enabled = false;
static bool color_initialized = false;

static bool detect_tty(void)
{
    return (isatty(fileno(stdout)) != 0);
}

static void ensure_initialized(void)
{
    if (!color_initialized) {
        color_enabled = detect_tty();
        color_initialized = true;
    }
}

void argparse_color_enable(bool enable)
{
    color_enabled = enable;
    color_initialized = true;
}

bool argparse_color_enabled(void)
{
    ensure_initialized();
    return color_enabled;
}

const char *argparse_color_code(argparse_color color)
{
    ensure_initialized();
    if (!color_enabled) return "";

    switch (color) {
    case ARGPARSE_COLOR_RED:     return "\033[31m";
    case ARGPARSE_COLOR_GREEN:   return "\033[32m";
    case ARGPARSE_COLOR_YELLOW:  return "\033[33m";
    case ARGPARSE_COLOR_BLUE:    return "\033[34m";
    case ARGPARSE_COLOR_MAGENTA: return "\033[35m";
    case ARGPARSE_COLOR_CYAN:    return "\033[36m";
    case ARGPARSE_COLOR_BOLD:    return "\033[1m";
    default:                     return "";
    }
}

void argparse_print_colored(FILE *fp, const char *text, argparse_color color)
{
    if (!fp || !text) return;

    ensure_initialized();

    if (!color_enabled || color == ARGPARSE_COLOR_DEFAULT) {
        fputs(text, fp);
        return;
    }

    const char *code = argparse_color_code(color);
    if (*code == '\0') {
        fputs(text, fp);
        return;
    }

    fputs(code, fp);
    fputs(text, fp);
    fputs("\033[0m", fp);
}
