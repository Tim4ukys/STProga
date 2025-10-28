#include "bit_copy.h"

void bitcpy(unsigned char* dst, size_t dst_ofs, unsigned char* src, size_t src_ofs, size_t bit_count) {
    if (!dst || !src || !bit_count) {
        return;
    }

    dst += dst_ofs / 8;
    src += src_ofs / 8;

    dst_ofs %= 8;
    src_ofs %= 8;

    for (size_t i = 0, k = 0, c = 0; i < bit_count; ++i, ++k, ++c) {
        if (src_ofs + k > 7) {
            src += 1;
            src_ofs = 0;
            k = 0;
        }
        if (dst_ofs + c > 7) {
            dst += 1;
            dst_ofs = 0;
            c = 0;
        }

        if ((*src >> (src_ofs + k)) & 1)
            *dst |= (1 << (dst_ofs + c));
        else
            *dst &= ~(1 << (dst_ofs + c));
    }
}

