#include <iostream>
#include <vector>

using namespace std;

class Base {
public:
    Base()
    {
        cout << "It is the constructer." << endl;
    }
    ~Base()
    {
        cout << "It is the destructor." << endl;
    }

    void* fun()
    {
        cout << "It is a normal public member fun." << endl;
        return NULL;
    }

    static void* testStaticFun()
    {
        cout << "It is the public static member function." << endl;
        return NULL;
    }

    void* normalMemFunCallStaticMemFun()
    {
        cout << "A normal member fun is Calling a static member fun." << endl;
        cout << "   ";
        testStaticFun();
        return NULL;
    }

    static void* staticMemFunCallNormalMemFun()
    {
        cout << "A static member function is Calling a normal member fun." << endl;
        cout << "   ";
        // this->fun();		// error: "It" is unavilable for static member functions.
        return NULL;
    }

    static void* testStaticWrapper(void *inst)
    {
        cout << "It is wrapper." << endl;
        cout << "   ";
        return (((Base *)(inst))->fun());
    }
};

int main()
{
    Base inst1, inst2;
    // test::testStaticFun();	// error: "test" is not a class name or namespace.
    Base::testStaticFun();
    Base::testStaticWrapper(&inst1);


    return 0;
}