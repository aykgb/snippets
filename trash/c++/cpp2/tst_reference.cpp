#include <iostream>
#include <string>

using namespace std;

union {
    int i;
    char x[2];
} u;

int main()
{
    int a[10];
    u.x[0] = 10;
    u.x[1] = 1;

    class C {

    };

    C c;
    std::cout << sizeof(c) << std::endl;
    std::cout << &c << std::endl;

    std::cout << u.i << std::endl;

    std::cout << "static stay hungry" << std::endl;

    return 0;
}
