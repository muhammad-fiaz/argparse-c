#ifndef ARGPARSE_TEST_FRAMEWORK_H
#define ARGPARSE_TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int _tests_run = 0;
static int _tests_passed = 0;
static int _tests_failed = 0;

#define ASSERT(expr) do { \
    if (!(expr)) { \
        printf(" [FAIL]\n"); \
        fprintf(stderr, "    ASSERT failed: %s\n    at %s:%d\n", #expr, __FILE__, __LINE__); \
        _tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        printf(" [FAIL]\n"); \
        fprintf(stderr, "    ASSERT_EQ failed: (%s) != (%s)\n    at %s:%d\n    got: %d, expected: %d\n", \
                #a, #b, __FILE__, __LINE__, (int)(a), (int)(b)); \
        _tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_NE(a, b) do { \
    if ((a) == (b)) { \
        printf(" [FAIL]\n"); \
        fprintf(stderr, "    ASSERT_NE failed: (%s) == (%s)\n    at %s:%d\n", \
                #a, #b, __FILE__, __LINE__); \
        _tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_STR_EQ(a, b) do { \
    if (strcmp((a), (b)) != 0) { \
        printf(" [FAIL]\n"); \
        fprintf(stderr, "    ASSERT_STR_EQ failed at %s:%d\n    got:      \"%s\"\n    expected: \"%s\"\n", \
                __FILE__, __LINE__, (a), (b)); \
        _tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_STR_NE(a, b) do { \
    if (strcmp((a), (b)) == 0) { \
        printf(" [FAIL]\n"); \
        fprintf(stderr, "    ASSERT_STR_NE failed at %s:%d\n    both: \"%s\"\n", \
                __FILE__, __LINE__, (a)); \
        _tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_NULL(ptr) do { \
    if ((ptr) != NULL) { \
        printf(" [FAIL]\n"); \
        fprintf(stderr, "    ASSERT_NULL failed: %s at %s:%d\n", #ptr, __FILE__, __LINE__); \
        _tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_NOT_NULL(ptr) do { \
    if ((ptr) == NULL) { \
        printf(" [FAIL]\n"); \
        fprintf(stderr, "    ASSERT_NOT_NULL failed: %s at %s:%d\n", #ptr, __FILE__, __LINE__); \
        _tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_TRUE(expr)  ASSERT(expr)
#define ASSERT_FALSE(expr) ASSERT(!(expr))

#define ASSERT_FLOAT_EQ(a, b) do { \
    if (fabsf((float)(a) - (float)(b)) > 1e-5f) { \
        printf(" [FAIL]\n"); \
        fprintf(stderr, "    ASSERT_FLOAT_EQ failed at %s:%d\n    got: %f, expected: %f\n", \
                __FILE__, __LINE__, (double)(a), (double)(b)); \
        _tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_DOUBLE_EQ(a, b) do { \
    if (fabs((double)(a) - (double)(b)) > 1e-9) { \
        printf(" [FAIL]\n"); \
        fprintf(stderr, "    ASSERT_DOUBLE_EQ failed at %s:%d\n    got: %f, expected: %f\n", \
                __FILE__, __LINE__, (double)(a), (double)(b)); \
        _tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_STR_CONTAINS(haystack, needle) do { \
    if (strstr((haystack), (needle)) == NULL) { \
        printf(" [FAIL]\n"); \
        fprintf(stderr, "    ASSERT_STR_CONTAINS failed at %s:%d\n    string: \"%s\"\n    missing: \"%s\"\n", \
                __FILE__, __LINE__, (haystack), (needle)); \
        _tests_failed++; \
        return; \
    } \
} while(0)

#define RUN_TEST(fn) do { \
    _tests_run++; \
    printf("  %-55s", #fn); \
    fn(); \
    _tests_passed++; \
    printf(" [PASS]\n"); \
} while(0)

#endif /* ARGPARSE_TEST_FRAMEWORK_H */
