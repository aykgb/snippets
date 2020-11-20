#include <iostream>
#include <vector>
#include <utility>

using namespace std;

int f()
{
    return 0;
}

int main()
{
    int&& r = f();
    cout << "&r before r = 21:" << &r << endl;
    r = 21;

    vector<int> v(100);
    //  int&& r1 = v[0];
    int& r1 = v[0];

    cout << r << endl;
    cout << &r << endl;
    cout << r1 << endl;
    cout << &r1 << endl;

    return 0;
}
