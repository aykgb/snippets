#include <iostream>
#include <functional>

int swap(int& a, int& b)
{
    a = a + b;
    b = a - b;
    a = a - b;

    return 0;
}

int main()
{
    int a[4];
    char b[256];
    int c = 1;
    int d = 2;

    std::cout << sizeof(a) << std::endl;
    std::cout << sizeof(&a) << std::endl;
    std::cout << sizeof(b) << std::endl;
    std::cout << sizeof(swap(c, d)) << std::endl;
    std::cout << c << " " << d << std::endl;

}
