#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    int a[5][4] = {
        2,  3,  0,  1,
        4,  5,  6,  7,
        15, 14, 17, 16,
        19, 18, 21, 20,
        8,  9, 12, 13
    };

    printf("a[0][0] %d %p\n", a[0][0], &a[0][0]);
    printf("a[0][1] %d %p\n", a[0][1], &a[0][1]);

    unsigned long long b = a[0];
    printf("a[0] %d %p\n", b << 24, &a[0]);

    return 0;
}
