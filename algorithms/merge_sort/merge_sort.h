#ifndef __ALG_MERGE_SORT_H__
#define __ALG_MERGE_SORT_H__

#include <inttypes.h>

typedef int(*merge_cmp_func)(const void*, const void*);

void merge_sort(void* arr, size_t lenght, size_t element_size, merge_cmp_func cmp_func);

#endif