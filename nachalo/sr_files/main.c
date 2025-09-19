#include <locale.h>
#include <stdio.h>
#include <inttypes.h>
#include <nmmintrin.h>

typedef struct {
	FILE* header;
	char fileName[260];
} stFile;

int main() {
	setlocale(LC_ALL, "Russian");
	stFile fFirst;
	stFile fSecond;
	
	printf("Введите название первого файла: ");
	scanf_s("%s", &fFirst.fileName, (uint32_t)sizeof(fFirst.fileName));
	if (fopen_s(&fFirst.header, fFirst.fileName, "rb")) {
		printf("Первого файла не существует.\n");
		return 0;
	}

	printf("Введите название второго файла: ");
	scanf_s("%s", fSecond.fileName, (uint32_t)sizeof(fSecond.fileName));
	if (fopen_s(&fSecond.header, fSecond.fileName, "rb")) {
		printf("Второго файла не существует.\n");
		return 0;
	}

	int state = 1;
	while (!feof(fFirst.header) || !feof(fSecond.header)) {
#define LEN_BUFF 128/8
		uint8_t buff_a[LEN_BUFF];
		uint8_t buff_b[LEN_BUFF];
		size_t r_first = fread(buff_a, 1, LEN_BUFF, fFirst.header);
		size_t r_second = fread(buff_b, 1, LEN_BUFF, fSecond.header);

		if (r_first != r_second) {
			state = 0;
			break;
		}

		__m128i strA = _mm_loadu_si128((__m128i*)buff_a);
		__m128i strB = _mm_loadu_si128((__m128i*)buff_b);
		
		register int result = _mm_cmpestri(strA, r_first, strB, r_second, _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH);
		if (result) {
			state = 0;
			break;
		}
	}
	fclose(fFirst.header);
	fclose(fSecond.header);

	printf(state ? "Файлы равны\n" : "Файлы не равны\n");
	return 0;
}