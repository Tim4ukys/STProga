#include <gtest/gtest.h>
#include <utility>
#include <string_view>

extern "C" {
#include <kmp_search.h>
}

extern "C" void kmp_init_lps(const char*, size_t*);

TEST(Algorithms_kmp_search, TestNullArgs) {
    EXPECT_FALSE(kmp_search(0, "test"));
    EXPECT_FALSE(kmp_search("test", 0));
    EXPECT_FALSE(kmp_search("\0", "\0"));
    EXPECT_FALSE(kmp_search("test", "a"));
    EXPECT_FALSE(kmp_search("tes", "test"));
}

TEST(Algorithms_kmp_search, TestLps) {
    std::pair<std::string_view, std::vector<size_t>> numbs[]{
        {"ababcabab", {0, 0, 1, 2, 0, 1, 2, 3, 4}},
        {"abcdabca", {0, 0, 0, 0, 1, 2, 3, 1}},
        {"aabaaac", {0, 1, 0, 1, 2, 2, 0}},
        {"aabaabaaaabaabаaab", {0, 1, 0, 1, 2, 3, 4, 5, 2, 2, 3, 4, 5, 6, 7, 8, 9, 3}}};

    for (auto& [pattern, ref_lps] : numbs) {
        std::vector<size_t> lps;
        lps.resize(ref_lps.size());

        kmp_init_lps(const_cast<char*>(pattern.data()), lps.data());
        EXPECT_FALSE(memcmp(lps.data(), ref_lps.data(), lps.size()));
    }
}

TEST(Algorithms_kmp_search, FindNeedle) {
    std::pair<std::string_view, std::vector<std::string_view>> tests[]{
        {"abcbecbabcbc", {"cbc", "abcbc", "e", "bab", "cbe", "abcbecbabcbc", "abcbecbabcbcdgdfhdgf", "test"}},
        {"aboba", {"ba", "bob", "oba", "o", "a", "test"}}};

    for (auto& [haystack, needle] : tests) {
        for (auto& i : needle) {
            auto        f = haystack.find(i);
            const char* p = kmp_search(haystack.data(), i.data());

            EXPECT_TRUE((f == std::string_view::npos && !p) ||
                        (p && f != std::string_view::npos) && (haystack.data() + f == p));
        }
    }
}