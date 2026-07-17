/**
 * @file argparse_color.h
 * @brief Colored terminal output support for help and error messages.
 *
 * Provides ANSI escape code support for printing colored text to the
 * terminal. Automatically detects TTY support and can be enabled/disabled
 * globally.
 */

#ifndef ARGPARSE_COLOR_H
#define ARGPARSE_COLOR_H

#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Available color codes for terminal output.
 */
typedef enum argparse_color {
    /** Reset to default terminal color. */
    ARGPARSE_COLOR_DEFAULT = 0,
    /** Red text. */
    ARGPARSE_COLOR_RED,
    /** Green text. */
    ARGPARSE_COLOR_GREEN,
    /** Yellow text. */
    ARGPARSE_COLOR_YELLOW,
    /** Blue text. */
    ARGPARSE_COLOR_BLUE,
    /** Magenta text. */
    ARGPARSE_COLOR_MAGENTA,
    /** Cyan text. */
    ARGPARSE_COLOR_CYAN,
    /** Bold text (may appear as default color with bold attribute). */
    ARGPARSE_COLOR_BOLD
} argparse_color;

/**
 * @brief Enable or disable colored output globally.
 *
 * When disabled, argparse_print_colored() outputs plain text without
 * escape sequences.
 *
 * @param[in] enable  true to enable color output, false to disable.
 */
void argparse_color_enable(bool enable);

/**
 * @brief Check if colored output is currently enabled.
 *
 * @return true if color output is enabled, false otherwise.
 */
bool argparse_color_enabled(void);

/**
 * @brief Get the ANSI escape sequence for a color.
 *
 * Returns the escape code string (e.g., "\\033[31m" for red) or an
 * empty string if color is disabled or the color is ARGPARSE_COLOR_DEFAULT.
 *
 * @param[in] color  The color to get the code for.
 *
 * @return Pointer to a static string containing the ANSI escape sequence,
 *         or an empty string for DEFAULT or when color is disabled.
 */
const char *argparse_color_code(argparse_color color);

/**
 * @brief Print colored text to a FILE stream.
 *
 * Outputs the text wrapped in the appropriate ANSI escape sequences.
 * If color is disabled or fp is not a TTY, outputs plain text.
 *
 * @param[in] fp     The output stream (e.g., stdout, stderr).
 * @param[in] text   The text to print. Must not be NULL.
 * @param[in] color  The color to use for the text.
 */
void argparse_print_colored(FILE *fp, const char *text, argparse_color color);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_COLOR_H */
