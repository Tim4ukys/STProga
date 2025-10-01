#include <gtest/gtest.h>
#include <Windows.h>
#include <ctime>

extern "C" {
#include <merge_sort.h>
}

static int __cdecl cmp_int(const void* a, const void* b) {
	return *(int*)b - *(int*)a;
}

static int __cdecl cmp_short(const void* a, const void* b) {
	return *(short*)b - *(short*)a;
}

static int __cdecl cmp_double(const void* a, const void* b) {
	//return (int)ceil(*(double*)b - *(double*)a);
	if (*(double*)b > *(double*)a) {
		return 1;
	}
	else if (*(double*)b < *(double*)a) {
		return -1;
	}
	return 0;
	//return *(double*)b > *(double*)a ? 1 : (*(double*)b < *(double*)a ? -1 : 0);
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
	constexpr size_t NUMBS_COUNT = 10'000u;
	srand(time(NULL));
	auto numbs_1 = (int*)malloc(NUMBS_COUNT * sizeof(int));
	auto numbs_2 = (int*)malloc(NUMBS_COUNT * sizeof(int));
	assert(numbs_1 && numbs_2 && "couldn't allocate memory");

	for (size_t i = 0; i < NUMBS_COUNT; i++) {
		numbs_1[i] = numbs_2[i] = rand() % 200'000 - 100'000;
	}

	merge_sort(numbs_1, NUMBS_COUNT, sizeof(*numbs_1), cmp_int);
	qsort(numbs_2, NUMBS_COUNT, sizeof(*numbs_1), cmp_int);
	EXPECT_FALSE(memcmp(numbs_1, numbs_2, NUMBS_COUNT * sizeof(int)));

	free(numbs_1);
	free(numbs_2);

	auto fnumbs_1 = (double*)malloc(NUMBS_COUNT * sizeof(double));
	auto fnumbs_2 = (double*)malloc(NUMBS_COUNT * sizeof(double));
	assert(fnumbs_1 && fnumbs_2 && "couldn't allocate memory");

	for (size_t i = 0; i < NUMBS_COUNT; i++) {
		fnumbs_1[i] = fnumbs_2[i] = 1.0 / (double)rand();
	}

	merge_sort(fnumbs_1, NUMBS_COUNT, sizeof(*fnumbs_1), cmp_double);
	qsort(fnumbs_2, NUMBS_COUNT, sizeof(*fnumbs_1), cmp_double);
	EXPECT_FALSE(memcmp(fnumbs_1, fnumbs_2, NUMBS_COUNT * sizeof(*fnumbs_1)));
	
	free(fnumbs_1);
	free(fnumbs_2);

	auto numbs16_1 = (uint16_t*)malloc(NUMBS_COUNT * sizeof(uint16_t));
	auto numbs16_2 = (uint16_t*)malloc(NUMBS_COUNT * sizeof(uint16_t));
	assert(numbs16_1 && numbs16_2 && "couldn't allocate memory");

	for (size_t i = 0; i < NUMBS_COUNT; i++) {
		numbs16_1[i] = numbs16_2[i] = uint16_t(rand() % 0xff'ff);
	}

	merge_sort(numbs16_1, NUMBS_COUNT, sizeof(short), cmp_short);
	qsort(numbs16_2, NUMBS_COUNT, sizeof(short), cmp_short);
	EXPECT_FALSE(memcmp(numbs16_1, numbs16_2, NUMBS_COUNT * sizeof(short)));

	free(numbs16_1);
	free(numbs16_2);
}