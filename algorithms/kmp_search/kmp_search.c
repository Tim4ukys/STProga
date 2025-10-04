#include "kmp_search.h"
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void kmp_init_lps(const char* pattern, size_t* lps) {
    if (!pattern || !*pattern || !lps)
        return;

    lps[0] = 0;

    for (size_t x = 0, y = 1; pattern[y];) {
        if (pattern[x] == pattern[y]) {
            lps[y++] = ++x;
        } else if (x) {
            x = lps[x - 1];
        } else {
            lps[y++] = 0;
        }
    }
}

const char* kmp_search(const char* str, const char* pattern) {
    if (!str || !pattern || !*str || !*pattern)
        return (char*)-1;

    const size_t szPattern = strlen(pattern);
    if (strlen(str) < szPattern)
        return (char*)-1;

    size_t* lps = malloc(sizeof(size_t) * szPattern);
    assert(lps && "couldn't allocate memory");
    kmp_init_lps(pattern, lps);

    for (size_t x = 0, y = 0; str[x];) {
        if (str[x] == pattern[y]) {
            ++x;
            if (++y >= szPattern) {
                free(lps);
                return str + x - szPattern;
            }
        } else if (y) {
            y = lps[y - 1];
        } else {
            ++x;
        }
    }
    free(lps);
    return (char*)-1;
}
