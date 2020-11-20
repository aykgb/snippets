#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

class A {
   public:
    void func1() { std::cout << "void func()" << std::endl; }

    int func2(int a) {
        std::cout << "int func(int)" << std::endl;
        return a;
    }
};

int main() {
    auto a_func1 = std::mem_fn(&A::func1);
    A a;
    a_func1(a);

    return 0;
}
