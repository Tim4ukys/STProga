#ifndef __ALG_BUBBLE_SORT_H__
#define __ALG_BUBBLE_SORT_H__

#include <inttypes.h>

typedef int(*bubble_cmp_func)(const void*, const void*);

void bubble_sort(void* arr, size_t lenght, size_t element_size, bubble_cmp_func cmp_func);

#endif