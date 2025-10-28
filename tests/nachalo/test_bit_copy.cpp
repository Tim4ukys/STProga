#include <gtest/gtest.h>
#include <utility>
#include <Windows.h>

extern "C" {
#include <bit_copy.h>
}

#define TEST_FUNC(dst, dst_offset, src, src_offset, bit_count, len, result) \
    {{(uint8_t*)dst, dst_offset, (uint8_t*)src, src_offset, bit_count}, len, (uint8_t*)result}

#pragma pack(push)
#pragma pack(1)
struct stTestArgs {
    unsigned char* a1;
    size_t         a2;
    unsigned char* a3;
    size_t         a4;
    size_t         a5;
};
#pragma pack(pop)

struct stTest {
    stTestArgs     m_Args;
    size_t         m_szLen;
    unsigned char* m_pResult;
};

TEST(Base_BitCopy, SimpleTests) {
    static stTest sArr[] = {
        TEST_FUNC("\x07\x00", 2, "\x66\xfd", 8, 7, 2, "\xf7\x01"),
        TEST_FUNC("\x55", 2, "\x51", 1, 6, 1, "\xA1"),
        TEST_FUNC("\xD3\xE0\x18\x65", 2, "\x8D\x78\x7F\x11", 5, 20, 4u, "\x13\xEf\x2f\x65")
    };

    for (auto& i : sArr) {
        DWORD oldProtect;
        VirtualProtect(i.m_Args.a1, i.m_szLen, PAGE_READWRITE, &oldProtect);
        bitcpy(i.m_Args.a1, i.m_Args.a2, i.m_Args.a3, i.m_Args.a4, i.m_Args.a5);
        VirtualProtect(i.m_Args.a1, i.m_szLen, oldProtect, 0);
        EXPECT_FALSE(memcmp(i.m_Args.a1, i.m_pResult, i.m_szLen));
    }
}

TEST(Base_BitCopy, NoValidArgs) {
    uint8_t a[] = "\x66\xfd";
    uint8_t b[] = "\x66\xfd";

    bitcpy(0, 0, 0, 0, 0);
    bitcpy(a, 0, 0, 0, 0);
    bitcpy(a, 0, b, 0, 0);
    EXPECT_FALSE(memcmp(a, b, 2));
}
