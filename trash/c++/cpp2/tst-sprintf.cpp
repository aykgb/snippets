#include <iostream>
#include <stdio.h>

using namespace std;

int main()
{
    char outp[50];
    char in = 121;
    sprintf(outp, "0x%03x", in);
    std::cout << outp << std::endl;
    return 0;
}
