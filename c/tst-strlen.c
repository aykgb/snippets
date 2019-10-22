#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char *p = (char *)malloc(sizeof(char) * 100);
    strcpy(p, "This is a test!\n");
    printf("%d\n", strlen(p));

    return 0;
}