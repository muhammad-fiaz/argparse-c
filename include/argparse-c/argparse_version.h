/**
 * @file argparse_version.h
 * @brief Version information for the argparse-c library.
 *
 * This header provides both compile-time version macros and runtime version
 * query functions. Use these to verify library compatibility at build time
 * or report the version at runtime.
 */

#ifndef ARGPARSE_VERSION_H
#define ARGPARSE_VERSION_H

/**
 * @def ARGPARSE_VERSION_MAJOR
 * @brief Major version number (breaking changes).
 *
 * Incremented when the library makes incompatible API changes.
 */

/**
 * @def ARGPARSE_VERSION_MINOR
 * @brief Minor version number (new functionality).
 *
 * Incremented when new functionality is added in a backwards-compatible manner.
 */

/**
 * @def ARGPARSE_VERSION_PATCH
 * @brief Patch version number (bug fixes).
 *
 * Incremented for backwards-compatible bug fixes.
 */

/**
 * @def ARGPARSE_VERSION_STRING
 * @brief String representation of the full version (e.g., "0.1.0").
 */

/**
 * @def ARGPARSE_VERSION_NUM
 * @brief Compute a single integer version number from components.
 *
 * @param major Major version component.
 * @param minor Minor version component.
 * @param patch Patch version component.
 * @return Combined version number as (major * 10000 + minor * 100 + patch).
 */

#define ARGPARSE_VERSION_MAJOR 0
#define ARGPARSE_VERSION_MINOR 1
#define ARGPARSE_VERSION_PATCH 0
#define ARGPARSE_VERSION_STRING "0.1.0"
#define ARGPARSE_VERSION_NUM(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the library version as a human-readable string.
 *
 * Returns a pointer to a static string such as "0.1.0". The returned pointer
 * is valid for the lifetime of the process and must not be freed.
 *
 * @return Pointer to the version string. Never NULL.
 *
 * @note The returned string is statically allocated and must not be modified.
 * @thread_safety Safe to call from multiple threads concurrently.
 */
const char *argparse_version_string(void);

/**
 * @brief Get the library version as individual integer components.
 *
 * @param[out] major Pointer to receive the major version number, or NULL.
 * @param[out] minor Pointer to receive the minor version number, or NULL.
 * @param[out] patch Pointer to receive the patch version number, or NULL.
 *
 * @note Any of the output parameters may be NULL if that component is not needed.
 * @thread_safety Safe to call from multiple threads concurrently.
 */
void argparse_version(int *major, int *minor, int *patch);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_VERSION_H */
