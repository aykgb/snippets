#include <iostream>
#include <string>

using namespace std;

void test_alignas()
{
    alignas(double) unsigned char c[1024];
    alignas(16) char d[100];

    std::cout << "alignof(int): " << alignof(int) << std::endl;
    std::cout << "  alignof(c): " << alignof(c) << std::endl;
    std::cout << "  alignof(d): " << alignof(d) << std::endl;
}

int main()
{
    test_alignas();
    return 0;
}
