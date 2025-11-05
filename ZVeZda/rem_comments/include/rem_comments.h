#ifndef __ZVEZDA_REM_COMMENTS_H__
#define __ZVEZDA_REM_COMMENTS_H__

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    RC_STATE_OK = 1,
    RC_STATE_BALANCE_ERR = 0
} RC_STATE;

typedef struct {
    uint8_t m_nOpen;
    bool    m_bIsOpenSimple;
    bool    m_bIsNewLine;
} stRCCheck;

void rc_reset(stRCCheck* p_check);
void rc_next(const char* str, size_t len, stRCCheck* p_check, const char* out, size_t* len_out);

RC_STATE rc_check(stRCCheck* p_check);


#endif
