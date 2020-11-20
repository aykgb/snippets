#include <iostream>
#include <vector>
#include <string>

#include <stdio.h>

using namespace std;

int main()
{
    std::string rep;
    rep.resize(12);
    std::cout << "size rep" << rep.size() << std::endl;
    printf("&rep[8] %p \n", &rep[8]);

    int32_t x = 0x12345678;
    std::cout << x << std::endl;
    char buf[4];
    memcpy(buf, &x, sizeof(x));
    printf("addr=%p, value=0x%x\n", buf, buf[0]);
    printf("addr=%p, value=0x%x\n", buf + 1, buf[1]);
    printf("addr=%p, value=0x%x\n", buf + 2, buf[2]);
    printf("addr=%p, value=0x%x\n", buf + 3, buf[3]);

    printf("0x%x\n", *(int32_t*)buf);
}
