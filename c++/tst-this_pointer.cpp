#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Base {
public:
    Base();
    ~Base();

    void test1();
    void test2();
    void test3(int tmp);
    void test4();

private:
    static int m_iStatic;
    int m_iTest;

};

int Base::m_iStatic = 1234;

void Base::test1()
{
    cout << "This is the test1() m_iStatic=" << m_iStatic << endl;
}

void Base::test2()
{
    cout << "This is the test2()" << endl;
}

void Base::test3(int tmp)
{
    cout << "This is the test3() tmp=" << tmp << endl;
}

void Base::test4()
{
    cout << "This is the test4() m_iTest=" << m_iTest << endl;
}

int main()
{
    Base *pNull = NULL;
    pNull->test1();
    pNull->test2();
    pNull->test3(100);
    pNull->test4();

    return 0;
}
