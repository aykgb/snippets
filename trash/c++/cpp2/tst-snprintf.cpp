#include <stdio.h>
#include <string.h>

#include <iostream>
#include <string>

std::string func()
{
    char version[8];
    int num = 0x1234;
    snprintf(version, 8, "%d.%d", num >> 8, num & 0xff);
    return version;
}
int main()
{
    std::cout << func() << std::endl;

    return 0;
}
