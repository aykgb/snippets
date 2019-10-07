#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class Test {
public:
    Test(int ii): i(ii)
    {
        cout << "#### Constructor Test()\n";
    }
    ~Test()
    {
        cout << "#### Destructor ~Test()\n";
    }

    void print()
    {
        cout << i << "\n";
    }

private:
    int i;
};

int main()
{
    cout << "Entering scope...\n";
    {
        auto p = make_shared<Test>(10);
    }
    cout << "Leaving the scope.\n";

    return 0;
}
