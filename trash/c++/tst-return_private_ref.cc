#include <iostream>
#include <string>

using namespace std;

class TEST {
    int x = 10;

   public:
    int& getRef() { return x; }
};

int main() {
    TEST tt;
    auto x = tt.getRef();
    std::cout << x << std::endl;
    x = 11;
    std::cout << x << std::endl;

    return 0;
}
