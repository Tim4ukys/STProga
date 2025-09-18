#include <locale.h>
#include <stdio.h>
#include <inttypes.h>
#include <malloc.h>

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
#define LEN_BUFF 256
		uint8_t buff_a[LEN_BUFF];
		uint8_t buff_b[LEN_BUFF];
		size_t r_first = fread(buff_a, 1, LEN_BUFF, fFirst.header);
		size_t r_second = fread(buff_b, 1, LEN_BUFF, fSecond.header);
		
		if (r_first != r_second){
			state = 0;
			break;
		}

		for (size_t i = 0u; i < r_first; i++) {
			if (buff_a[i] != buff_b[i]) {
				state = 0;
				break;
			}
		}
	}
	fclose(fFirst.header);
	fclose(fSecond.header);

	printf(state ? "Файлы равны\n" : "Файлы не равны\n");
	return 0;
}