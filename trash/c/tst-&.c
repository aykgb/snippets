#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int test(int **p)
{
    *p = (int *)malloc(sizeof(int));
    return 0;
}

int main()
{
    int a;
    int *p = &a;
    printf("%p\n", p);
    test(&p);
    printf("%p\n", p);

    char msg[] = {0x01, 0x02, 0x03, 0x04};	// 内存由低到高存储
    printf("%I64u\n", sizeof(msg));
    printf("%I64u\n", sizeof(int));

    printf("%x\n", *((int *)msg));

    printf("%p\n", msg);
    printf("%p\n", (msg + 1));

    return 0;
}