#include <stdio.h>
#include <stdlib.h>

int main()
{
    int a;
    a = 100;

    printf("0x%x\n", (&a));
    // printf("0x%x\n", &(&a));

    return 0;
}