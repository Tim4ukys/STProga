#include "rem_comments.h"
#include <string.h>

void rc_reset(stRCCheck* p_check) {
    memset(p_check, 0, sizeof(stRCCheck));
    p_check->m_bIsEmptyStr = true;
}

void rc_next(const char* str, size_t len, stRCCheck* p_check, char* out, size_t* len_out) {
    if (len_out)
        *len_out = 0;

    for (size_t i = 0; i < len; i++) {
        char a = str[i];

        if (p_check->m_currentState) {
            if (p_check->m_currentState == STATE_KOM_SLASH) {
                if (a == '/') {
                    p_check->m_bIsOpenSimple = true;
                    p_check->m_currentState = STATE_KOM_NONE;
                }
                else if (a == '*')
                    p_check->m_currentState = STATE_KOM_STAR_1;
                else if (out) {
                    *out++ = '/';
                    (*len_out)++;
                    p_check->m_currentState = STATE_KOM_NONE;
                }
            } else if (p_check->m_currentState == STATE_KOM_STAR_2) {
                p_check->m_currentState = (a == '/' ? STATE_KOM_NONE : STATE_KOM_STAR_1);
                continue;
            } else if (a == '*') {
                p_check->m_currentState = STATE_KOM_STAR_2;
            }
        } else if (a == '\\') {
            p_check->m_bIsNewLine = true;
        } else if (p_check->m_bIsNewLine) {
            if (a == '\n')
                p_check->m_bIsNewLine = false;
        } else if (p_check->m_bIsOpenSimple) {
            if (a == '\n') {
                p_check->m_bIsOpenSimple = false;
                if (p_check->m_bIsEmptyStr)
                    continue;
            }
        } else if (a == '/') {
            p_check->m_currentState = 1;
        }

        if (out && !p_check->m_currentState && !p_check->m_bIsOpenSimple) {
            *out++ = a;
            (*len_out)++;
            p_check->m_bIsEmptyStr = false;
            if (a == '\n') {
                p_check->m_bIsEmptyStr = true;
            }
        }
    }
}

RC_STATE rc_check(stRCCheck* p_check) {
    return p_check->m_currentState ? RC_STATE_BALANCE_ERR : RC_STATE_OK;
}

