#include "rem_comments.h"
#include <string.h>

void rc_reset(stRCCheck* p_check) {
    memset(p_check, 0, sizeof(stRCCheck));
}

void rc_next(const char* str, size_t len, stRCCheck* p_check, char* out, size_t* len_out) {
    if (len_out)
        *len_out = 0;

    for (size_t i = 0; i < len; i++) {
        char a = str[i];

        if (p_check->m_nOpen) {
            if (p_check->m_nOpen == 1) {
                if (a == '/') {
                    p_check->m_bIsOpenSimple = true;
                    p_check->m_nOpen = 0;
                }
                else if (a == '*')
                    p_check->m_nOpen = 2;
                else if (out) {
                    *out++ = '/';
                    (*len_out)++;
                    p_check->m_nOpen = 0;
                }
            } else if (p_check->m_nOpen == 3) {
                p_check->m_nOpen = (a == '/' ? 0 : 2);
                continue;
            } else if (a == '*') {
                p_check->m_nOpen = 3;
            }
        } else if (a == '\\') {
            p_check->m_bIsNewLine = true;
        } else if (p_check->m_bIsNewLine) {
            if (a == '\n')
                p_check->m_bIsNewLine = false;
        } else if (p_check->m_bIsOpenSimple) {
            if (a == '\n') {
                p_check->m_bIsOpenSimple = false; 
            }
        } else if (a == '/') {
            p_check->m_nOpen = 1;
        }

        if (out && !p_check->m_nOpen && !p_check->m_bIsOpenSimple) {
            *out++ = a;
            (*len_out)++;
        }
    }
}

RC_STATE rc_check(stRCCheck* p_check) {
    return p_check->m_nOpen ? RC_STATE_BALANCE_ERR : RC_STATE_OK;
}

