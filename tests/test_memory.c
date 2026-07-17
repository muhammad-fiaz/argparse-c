#include "test_framework.h"
#include <argparse-c/argparse.h>
#include <stdlib.h>
#include <string.h>

static int alloc_count = 0;
static int free_count = 0;

static void *counting_alloc(size_t size, void *data) {
    (void)data;
    alloc_count++;
    return malloc(size);
}

static void counting_free(void *ptr, void *data) {
    (void)data;
    if (ptr) {
        free_count++;
        free(ptr);
    }
}

static void test_default_allocator(void) {
    const argparse_allocator *a = argparse_default_allocator();
    ASSERT_NOT_NULL(a);
    ASSERT_NOT_NULL(a->alloc);
    ASSERT_NOT_NULL(a->free);
}

static void test_custom_allocator_called(void) {
    const argparse_allocator *original = argparse_default_allocator();
    alloc_count = 0;
    free_count = 0;
    static argparse_allocator alloc = {0};
    alloc.alloc = counting_alloc;
    alloc.free = counting_free;
    alloc.user_data = NULL;
    argparse_set_default_allocator(&alloc);

    struct argparse *p = argparse_new("prog", NULL);
    ASSERT_NOT_NULL(p);
    argparse_add_option(p, 'v', "verbose", ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose", "verbose");
    const char *argv[] = {"prog", "-v"};
    struct argparse_result *r = argparse_parse(p, 2, argv);
    ASSERT_NOT_NULL(r);
    argparse_result_free(r);
    argparse_free(p);

    /* Restore original allocator */
    argparse_set_default_allocator(original);
}

static void test_alloc_and_free_directly(void) {
    void *ptr = argparse_alloc(64);
    ASSERT_NOT_NULL(ptr);
    argparse_mem_free(ptr);
}

static void test_alloc_zero(void) {
    void *ptr = argparse_alloc(0);
    /* Behavior is implementation-defined; just ensure no crash */
    argparse_mem_free(ptr);
}

static void test_free_null_direct(void) {
    argparse_mem_free(NULL);
}

static void test_restore_default_allocator(void) {
    const argparse_allocator *original = argparse_default_allocator();
    ASSERT_NOT_NULL(original);

    argparse_allocator custom = {counting_alloc, counting_free, NULL};
    argparse_set_default_allocator(&custom);

    const argparse_allocator *after_custom = argparse_default_allocator();
    ASSERT_NOT_NULL(after_custom);

    /* Restore */
    argparse_set_default_allocator(original);
    const argparse_allocator *restored = argparse_default_allocator();
    ASSERT_NOT_NULL(restored);
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    printf("\n=== test_memory ===\n");
    _tests_run = 0; _tests_passed = 0; _tests_failed = 0;

    RUN_TEST(test_default_allocator);
    RUN_TEST(test_custom_allocator_called);
    RUN_TEST(test_alloc_and_free_directly);
    RUN_TEST(test_alloc_zero);
    RUN_TEST(test_free_null_direct);
    RUN_TEST(test_restore_default_allocator);

    printf("\nResults: %d/%d passed", _tests_passed, _tests_run);
    if (_tests_failed > 0) printf(", %d FAILED", _tests_failed);
    printf("\n");
    return _tests_failed > 0 ? 1 : 0;
}
