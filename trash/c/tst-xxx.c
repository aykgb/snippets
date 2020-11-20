#include <stdio.h>
#include <string.h>

int test()
{
    return 0;
}

int main()
{
    if(test() == 0) {
        printf("Test is passed!\n");
    }

    int i = 0;
    char tmp[] = "0123456789\n";
    tmp[i++] = tmp[i];
    printf(tmp);

    char temp[100] = "This is\0";
    strcpy(temp, "This is a test!\n");
    printf(temp);

    return 0;
}