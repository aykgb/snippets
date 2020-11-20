#include <iostream>
#include <vector>

using namespace std;

int main() {
    int status = 1;
    switch (status) {
       case 0:
           std::cout << "case 0\n";
       case 1:
           std::cout << "case 1\n";
       case 2:
           std::cout << "case 2\n";
       default:
           std::cout << "default\n";
           break;
    }

    return 0;
}
