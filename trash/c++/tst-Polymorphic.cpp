#include <iostream>
#include <vector>

using namespace std;

class Base {
public:
    Base()
    {
        cout << "It's Base's constructor.\n" << endl;
    }
    ~Base()
    {
        cout << "It's Base's destructor.\n" << endl;
    }

    virtual void callFun();
    virtual void fun();
};

void Base::fun()
{
    cout << "This is Base's fun function." << endl;
}

void Base::callFun()
{
    fun();
}

class Derive: public Base {
public:
    Derive()
    {
        cout << "It's Derive's constructor.\n" << endl;
    }
    ~Derive()
    {
        cout << "It's Derive's destructor.\n" << endl;
    }

    void fun();
};

void Derive::fun()
{
    cout << "This is Derive's fun function." << endl;
}

int main()
{
    Derive Test;
    // Base Test;
    Test.callFun();	// 使用子类指针调用父类函数，给出不同的行为。

    return 0;
}