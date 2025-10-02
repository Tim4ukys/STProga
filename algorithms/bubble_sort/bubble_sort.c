#include "bubble_sort.h"
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <stdbool.h>

void bubble_sort(void* arr, size_t lenght, size_t element_size, bubble_cmp_func cmp_func) {
	if (lenght <= 1 || !cmp_func || !arr || !element_size)
		return;

	void* pTemp = malloc(element_size);
	assert(pTemp && "couldn't allocate memory");

	do  {
		for (size_t i = 0; i < lenght - 1;) {
			void* pA = (uint8_t*)(arr)+element_size * i++;
			void* pB = (uint8_t*)(arr)+element_size * i;

			if (cmp_func(pA, pB) > 0) {
				memcpy(pTemp, pB, element_size);
				memcpy(pB, pA, element_size);
				memcpy(pA, pTemp, element_size);
			}
		}
	} while (1 <= --lenght);

	free(pTemp);
}
