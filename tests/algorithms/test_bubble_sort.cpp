#include <gtest/gtest.h>
#include <Windows.h>
#include <ctime>
#include <initializer_list>

extern "C" {
#include <bubble_sort.h>
}

static int __cdecl cmp_int(const void* a, const void* b) {
	return *(int*)b - *(int*)a;
}

static int g_arrNumbs[]{ 228, 2022, 02, 24, -11 };
static int g_arrNumbsOrig[]{ 228, 2022, 02, 24, -11 };
static int g_arrNumbsSorted[]{2022, 228, 24, 2, -11};

TEST(Algorithms_Bubble_Sort, OnlyNull) {
	bubble_sort(nullptr, 0, 0, nullptr);

	bubble_sort(g_arrNumbs, ARRAYSIZE(g_arrNumbs), 0, cmp_int);
	bubble_sort(g_arrNumbs, 0, sizeof(int), cmp_int);
	bubble_sort(g_arrNumbs, 1, sizeof(int), cmp_int);
	bubble_sort(g_arrNumbs, ARRAYSIZE(g_arrNumbs), sizeof(int), nullptr);
	EXPECT_FALSE(memcmp(g_arrNumbs, g_arrNumbsOrig, sizeof(g_arrNumbs)));
}

TEST(Algorithms_Bubble_Sort, Baza) {
	bubble_sort(g_arrNumbs, ARRAYSIZE(g_arrNumbs), sizeof(int), cmp_int);
	EXPECT_FALSE(memcmp(g_arrNumbs, g_arrNumbsSorted, sizeof(g_arrNumbs)));
}

TEST(Algorithms_Bubble_Sort, RandomNumbs) {
	constexpr size_t NUMBS_COUND =	10'000u;
	srand(time(NULL));
	auto numbs_1 = (int*)malloc(NUMBS_COUND * sizeof(int));
	auto numbs_2 = (int*)malloc(NUMBS_COUND * sizeof(int));
	assert(numbs_1 && numbs_2 && "couldn't allocate memory");

	for (size_t i = 0; i < NUMBS_COUND; i++) {
		numbs_1[i] = numbs_2[i] = rand() % 200'000 - 100'000;
	}
	
	bubble_sort(numbs_1, NUMBS_COUND, sizeof(*numbs_1), cmp_int);
	qsort(numbs_2, NUMBS_COUND, sizeof(*numbs_1), cmp_int);
	EXPECT_FALSE(memcmp(numbs_1, numbs_2, NUMBS_COUND*sizeof(int)));

	free(numbs_1);
	free(numbs_2);
}