#ifndef __ZVEZDA_REM_COMMENTS_H__
#define __ZVEZDA_REM_COMMENTS_H__

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    RC_STATE_OK = 1,
    RC_STATE_BALANCE_ERR = 0
} RC_STATE;


typedef enum {
    STATE_KOM_NONE = 0,
    STATE_KOM_SLASH = 1,
    STATE_KOM_STAR_1 = 2,
    STATE_KOM_STAR_2 = 3
} STATE_KOM;

typedef struct {
    STATE_KOM m_currentState;
    bool      m_bIsOpenSimple;
    bool      m_bIsNewLine;
    bool      m_bIsEmptyStr;
} stRCCheck;

void rc_reset(stRCCheck* p_check);
void rc_next(const char* str, size_t len, stRCCheck* p_check, const char* out, size_t* len_out);

RC_STATE rc_check(stRCCheck* p_check);


#endif
