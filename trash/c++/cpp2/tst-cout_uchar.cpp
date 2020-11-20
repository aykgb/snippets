#include <iostream>
#include <string>

using namespace std;

int main()
{
    unsigned char rxBuf[] = {0x79, 'X', 'X'};

    std::cout << rxBuf[0] << std::endl;
    std::cout << rxBuf[1] << std::endl;
    std::cout << rxBuf[2] << std::endl;

    return 0;
}
