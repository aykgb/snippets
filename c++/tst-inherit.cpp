#include <iostream>

using namespace std;

class Base {
public:
    Base()
    {
        cout << "It's a constructor.\n" << endl;
    }
    ~Base()
    {
        cout << "It's a destructor.\n" << endl;
    }

    bool init();
    void fun();

protected:
    const char *str;

private:
    int var_i;
};

void Base::fun()
{
    cout << "It's a normal member function.\n" << endl;
}

bool Base::init()
{
    var_i = 10;
    str = "  This is a const string.";

    return false;
}

class Test : public Base {
public:
    Test()
    {
        Base::init();
        cout << "  It's a constructor of inherited class.\n" << endl;
    }
    ~Test()
    {
        cout << "  It's a destructor of inherited class.\n" << endl;
    }

    void fun()
    {
        cout << str << endl;
    }
};

int main()
{
    Test testInst;
    testInst.fun();

    return 0;
}
