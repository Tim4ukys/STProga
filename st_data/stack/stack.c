#include "stack.h"
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdbool.h>

#define MIN_LEN 8

static bool is_valid(Stack* pstack) {
    return pstack && pstack->m_szElement && pstack->m_szLen;
}

void stack_create(Stack* pstack) {
    if (!pstack || !pstack->m_szElement)
        return;

    pstack->m_nLastIndex = 0;
    if (pstack->m_arr && pstack->m_szLen != MIN_LEN)
        pstack->m_arr = realloc(pstack->m_arr, MIN_LEN * pstack->m_szElement);
    else 
        pstack->m_arr = malloc(MIN_LEN * pstack->m_szElement); 
    
    assert(pstack->m_arr && "couldn't allocate memory");
    pstack->m_szLen = MIN_LEN;
}

void stack_destroy(Stack* pstack) {
    if (!pstack)
        return;
    if (pstack->m_arr)
        free(pstack->m_arr);

    memset(pstack, 0, sizeof(Stack));
}

static void keep_memory_strong(Stack* pstack) {
    if (3 * pstack->m_nLastIndex > pstack->m_szLen * 2) {   
        pstack->m_arr = realloc(pstack->m_arr, (pstack->m_szLen *= 2)*pstack->m_szElement);
    } else if (pstack->m_szLen == MIN_LEN) {
        return;
    } else if (3 * pstack->m_nLastIndex < pstack->m_szLen) {
        pstack->m_arr = realloc(pstack->m_arr, (pstack->m_szLen /= 2) * pstack->m_szElement);
    }
    assert(pstack->m_arr && "couldn't allocate memory");
}

void stack_push(Stack* pstack, Pointer value) {
    if (!is_valid(pstack) || !value)
        return;
    memcpy(pstack->m_arr + pstack->m_szElement*pstack->m_nLastIndex++, value, pstack->m_szElement);
    keep_memory_strong(pstack);
}

size_t stack_size(Stack* pstack) {
    return is_valid(pstack) ? pstack->m_nLastIndex : 0;
}

Pointer stack_pop(Stack* pstack) {
    if (!is_valid(pstack) || !pstack->m_nLastIndex)
        return NULL;
    
    pstack->m_nLastIndex--;
    keep_memory_strong(pstack);
    return pstack->m_arr + pstack->m_szElement * pstack->m_nLastIndex;
}

Pointer stack_peek(Stack* pstack) {
    if (!is_valid(pstack) || !pstack->m_nLastIndex)
        return NULL;

    return pstack->m_arr + pstack->m_szElement * (pstack->m_nLastIndex - 1);
}