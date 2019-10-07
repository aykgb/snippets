#include <iostream>

using namespace std;

enum { a, b, c, d };

int main() {
    if(a < 5) {
        std::cout << "using enum a is ok." << std::endl;
    }
    return 0;
}
