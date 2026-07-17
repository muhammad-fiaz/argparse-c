/**
 * @file argparse_memory.h
 * @brief Custom allocator support for the argparse-c library.
 *
 * Provides a mechanism to replace the default memory allocator with a
 * custom implementation. This is useful for embedded systems, debugging,
 * or integrating with application-specific memory pools.
 */

#ifndef ARGPARSE_MEMORY_H
#define ARGPARSE_MEMORY_H

#include "argparse_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Memory allocator interface.
 *
 * Holds function pointers for custom allocation and deallocation,
 * along with an opaque user data pointer that is passed through
 * to the functions on each call.
 */
typedef struct argparse_allocator {
    /** @brief Allocation function. Must not be NULL. */
    argparse_alloc_fn alloc;

    /** @brief Deallocation function. Must not be NULL. */
    argparse_free_fn free;

    /** @brief Opaque pointer passed to alloc and free functions. */
    void *user_data;
} argparse_allocator;

/**
 * @brief Get the current default allocator.
 *
 * Returns a pointer to the default allocator configuration. If no custom
 * allocator has been set, this returns the built-in standard library
 * allocator (malloc/free).
 *
 * @return Pointer to the current default allocator. Never NULL.
 *
 * @note The returned pointer is valid until argparse_set_default_allocator()
 *       is called. It must not be freed by the caller.
 * @thread_safety Not thread-safe. The default allocator is global state.
 */
const argparse_allocator *argparse_default_allocator(void);

/**
 * @brief Set the default allocator for all subsequent parser operations.
 *
 * Replaces the global default allocator. All parsers created after this
 * call will use the new allocator. Existing parsers are not affected.
 *
 * @param[in] allocator Pointer to the new allocator configuration. Must not
 *                      be NULL. The allocator struct is copied internally,
 *                      so the caller retains ownership of the original.
 *
 * @note This function affects all parsers globally, not just new ones.
 * @thread_safety Not thread-safe. The default allocator is global state.
 * @warning Changing the allocator while parsers are active may cause
 *          undefined behavior if the old and new allocators are incompatible.
 */
void argparse_set_default_allocator(const argparse_allocator *allocator);

/**
 * @brief Allocate memory using the current default allocator.
 *
 * This is an internal convenience function. Most users do not need to call
 * this directly.
 *
 * @param[in] size Number of bytes to allocate.
 * @return Pointer to allocated memory, or NULL on allocation failure.
 *
 * @thread_safety Thread-safe if the underlying allocator is thread-safe.
 */
void *argparse_alloc(size_t size);

/**
 * @brief Free memory using the current default allocator.
 *
 * This is an internal convenience function. Most users do not need to call
 * this directly.
 *
 * @param[in] ptr Pointer to memory to free. May be NULL (no-op).
 *
 * @thread_safety Thread-safe if the underlying allocator is thread-safe.
 */
void argparse_mem_free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif /* ARGPARSE_MEMORY_H */
