#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct my {
    char ch;
    int a;
    double b;
    struct {
        char c;
        char d;
        int e;
    };
} __attribute__ ((packed));

int main()
{

    printf("sizeof(int): %ld\n", (long)sizeof(int));
    printf("sizeof(my): %lu\n", (long)sizeof(my));

    return 0;
}