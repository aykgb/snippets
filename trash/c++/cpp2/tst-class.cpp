#include <iostream>
#include <memory>

using namespace std;


class Test {
public:
    Test(const int a):p_int(new int(a)) { }
    void print()
    {
        std::cout << "*p_int: " << *p_int << std::endl;
    }
    int* get()
    {
        return p_int;
    }
private:
    //  shared_ptr<int> p_int;
    int* p_int; // Test a(10), b(11); a = b; 将会导致内存泄漏
};

int main()
{
    Test a(10);
    Test b(11);

    b = a;

    a.print();
    b.print();

    int* p = nullptr;
    {
        Test c(12); // 隐式销毁内置指针成员不会delete它所指向的对象
        p = c.get();
    }

    std::cout << "*p:" << *p << std::endl;

    return 0;
}
