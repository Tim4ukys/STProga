#include <gtest/gtest.h>
#include <Windows.h>
#include <ctime>

extern "C" {
#include <merge_sort.h>
}

static int __cdecl cmp_int(const void* a, const void* b) {
	return *(int*)b - *(int*)a;
}

static int g_arrNumbs[]{ -11, 2022, 02, 24, 228 };
static int g_arrNumbsOrig[]{ -11, 2022, 02, 24, 228 };
static int g_arrNumbsSorted[]{ 2022, 228, 24, 2, -11 };

TEST(Algorithms_Merge_Sort, OnlyNull) {
	merge_sort(nullptr, 0, 0, nullptr);

	merge_sort(g_arrNumbs, ARRAYSIZE(g_arrNumbs), 0, cmp_int);
	merge_sort(g_arrNumbs, 0, sizeof(int), cmp_int);
	merge_sort(g_arrNumbs, 1, sizeof(int), cmp_int);
	merge_sort(g_arrNumbs, ARRAYSIZE(g_arrNumbs), sizeof(int), nullptr);
	EXPECT_FALSE(memcmp(g_arrNumbs, g_arrNumbsOrig, sizeof(g_arrNumbs)));
}

TEST(Algorithms_Merge_Sort, Baza) {
	merge_sort(g_arrNumbs, ARRAYSIZE(g_arrNumbs), sizeof(int), cmp_int);
	EXPECT_FALSE(memcmp(g_arrNumbs, g_arrNumbsSorted, sizeof(g_arrNumbs)));
}

TEST(Algorithms_Merge_Sort, RandomNumbs) {
	constexpr size_t NUMBS_COUND = 10'000u;
	srand(time(NULL));
	auto numbs_1 = (int*)malloc(NUMBS_COUND * sizeof(int));
	auto numbs_2 = (int*)malloc(NUMBS_COUND * sizeof(int));
	assert(numbs_1 && numbs_2 && "couldn't allocate memory");

	for (size_t i = 0; i < NUMBS_COUND; i++) {
		numbs_1[i] = numbs_2[i] = rand() % 200'000 - 100'000;
	}

	merge_sort(numbs_1, NUMBS_COUND, sizeof(*numbs_1), cmp_int);
	qsort(numbs_2, NUMBS_COUND, sizeof(*numbs_1), cmp_int);
	EXPECT_FALSE(memcmp(numbs_1, numbs_2, NUMBS_COUND * sizeof(int)));

	free(numbs_1);
	free(numbs_2);
}