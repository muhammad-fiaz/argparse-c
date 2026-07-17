#include "argparse_internal.h"

#include <stdlib.h>
#include <string.h>

bool argparse_starts_with(const char *str, const char *prefix)
{
    if (!str || !prefix) {
        return false;
    }
    size_t prefix_len = strlen(prefix);
    if (prefix_len == 0) {
        return true;
    }
    return strncmp(str, prefix, prefix_len) == 0;
}

bool argparse_streq(const char *a, const char *b)
{
    if (!a && !b) return true;
    if (!a || !b) return false;
    return strcmp(a, b) == 0;
}

int argparse_levenshtein_distance(const char *s1, const char *s2)
{
    if (!s1 && !s2) return 0;
    if (!s1) return (int)strlen(s2);
    if (!s2) return (int)strlen(s1);

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    size_t alloc_size = (len1 + 1) * (len2 + 1) * sizeof(int);
    if (alloc_size / sizeof(int) != (len1 + 1) * (len2 + 1)) {
        return -1;
    }

    int *dp = (int *)malloc(alloc_size);
    if (!dp) {
        return -1;
    }

    #define DP(i, j) dp[(i) * (len2 + 1) + (j)]

    for (size_t i = 0; i <= len1; i++) {
        DP(i, 0) = (int)i;
    }
    for (size_t j = 0; j <= len2; j++) {
        DP(0, j) = (int)j;
    }

    for (size_t i = 1; i <= len1; i++) {
        for (size_t j = 1; j <= len2; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            int del = DP(i - 1, j) + 1;
            int ins = DP(i, j - 1) + 1;
            int sub = DP(i - 1, j - 1) + cost;
            int min = del;
            if (ins < min) min = ins;
            if (sub < min) min = sub;
            DP(i, j) = min;
        }
    }

    int result = DP(len1, len2);
    #undef DP

    free(dp);
    return result;
}
