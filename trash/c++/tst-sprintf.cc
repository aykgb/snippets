#include <iostream>
#include <stdio.h>

using namespace std;

int main() {
    uint64_t value = 1024;
    char buffer[32];
    snprintf(buffer, 32, "%016lx", value);
    std::cout << buffer << std::endl;
    return 0;
}
