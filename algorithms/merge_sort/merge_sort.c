#include "merge_sort.h"
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <stdbool.h>

#define get(p, i, sz) (uint8_t*)(p)+(i)*(sz)

void merge_sort(void* arr, size_t lenght, size_t element_size, merge_cmp_func cmp_func)
{
	if (lenght <= 1 || !cmp_func || !arr || !element_size)
		return;

	for (size_t pow = 2; pow / 2 < lenght; pow *= 2) {
		void* pTemp = malloc(pow * element_size);
		assert(pTemp && "couldn't allocate memory");
		
		// block - индекс в массиве начала блока размера pow
		for (size_t block = 0; block < lenght; block += pow) {
			// если другая половина блока выходит за размеры массива, то очевидно
			// что этот блок отсортирован был ранее
			if (block + pow / 2 >= lenght)
				break;

			for (size_t a = 0, b = pow / 2, i = 0; i < pow;) {
				void* pA = get(arr, block + a, element_size);
				void* pB = get(arr, block + b, element_size);
				if (b < pow && cmp_func(pA, pB) > 0) {
					memcpy(get(pTemp, i++, element_size), pB, element_size);
					b++;
					if (b + block == lenght || b == pow) {
						memcpy(get(pTemp, i, element_size), pA, element_size * (pow / 2 - a));
						break;
					}
				}
				else if (a < pow / 2) {
					memcpy(get(pTemp, i++, element_size), pA, element_size);
					a++;
					if (a == pow / 2) {
						memcpy(get(pTemp, i, element_size), pB, element_size * (pow - b));
						break;
					}
				}
			}
			
			memcpy(get(arr, block, element_size), pTemp, element_size * (block + pow > lenght ? lenght - block : pow));
		}
		
		free(pTemp);
	}
}
