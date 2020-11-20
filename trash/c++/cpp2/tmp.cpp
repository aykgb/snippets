/*************************************************************************
    > File Name: tmp.cpp
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: Wed 22 Jun 2016 05:38:43 PM CST
 ************************************************************************/

#include <iostream>
#include <bitset>

using namespace std;

int main()
{
    unsigned char bits8 = 0xFF;
    bool failure = true;

    std::cout << bitset<8>(bits8 ^ 0b00000010) << std::endl;
    std::cout << bitset<8>(bits8 & 0b00000010) << std::endl;
    if(failure != static_cast<bool>(bits8 & 0b00000010)) {
        std::cout << "flag" << std::endl;
    }
    std::cout << "end" << std::endl;

    return 0;
}
