#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include <errno.h>

int main(int argc, char* argv[]) {

    if(argc < 2) {
        fprintf(stderr, "two parameters required, provided 1.\n");
        exit(EXIT_FAILURE);
    }
    const char *in_str = argv[1];

    char *endptr;
    long val = strtol(in_str, &endptr, 10);  //10 的意思是 10 进制
    // 如果无法转换
    if (endptr == in_str) {
        fprintf(stderr, "No digits were found\n");
        exit(EXIT_FAILURE);
    }

    // 如果整型溢出了
    if (errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) {
        fprintf(stderr, "ERROR: number out of range for LONG\n");
        exit(EXIT_FAILURE);
     }

    // 如果是其它错误
    if (errno != 0 && val == 0) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }

    printf("input:%s , result:%ld\n", in_str, val);
}

