#include <stdio.h>
#include <stdlib.h>
#include "include/rem_comments.h"

#define LEN_BUFF 256

#define ERR_ARGS "Err args. Pls, run like: ./rem_comments_ex *in file name* *out file name*\n"
#define ERR_FILE_OPEN "Сouldn't open file. Abort.\n"
#define ERR_FILE_SV_OPEN "Couldn't save out file. Abort.\n"
#define ERR_NO_WRITE     "Couldn't write in out file. Abort.\n"
#define ERR_BALANCE      "There are no closed comments in the file. Out file no save."

void err_exit(const char* msg) {
    printf(msg);
    exit(-1);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        err_exit(ERR_ARGS); 
	}
    FILE* a = fopen(argv[1], "r"), *b;
    if (!a) {
        err_exit(ERR_FILE_OPEN);
    } else if (!(b = fopen(argv[2], "w"))) {
        err_exit(ERR_FILE_SV_OPEN);
    }

    stRCCheck rc;
    rc_reset(&rc);
    while (!feof(a)) {
        char buff[LEN_BUFF], buff_out[LEN_BUFF];
        size_t len = fread(buff, 1, LEN_BUFF, a), out_len = 0;

        rc_next(buff, len, &rc, buff_out, &out_len);
        if (fwrite(buff_out, 1, out_len, b) != out_len) {
            fclose(a);
            fclose(b);
            err_exit(ERR_NO_WRITE);
        }
    }
	fclose(a);
    fclose(b);

    switch (rc_check(&rc)) {
    case RC_STATE_OK:
        printf("success\n");
        break;
    case RC_STATE_BALANCE_ERR:
        remove(argv[2]);
        err_exit(ERR_BALANCE);
    }

	return 0;
}