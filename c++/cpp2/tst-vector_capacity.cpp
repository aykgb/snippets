#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int> vint(10, 5);
    //  vint.reserve(100);
    cout << "size of vint: " << vint.size() << "\n";
    cout << "capacity of vint: " << vint.capacity() << "\n";
    cout << "push back operation. \n";
    vint.push_back(1);
    cout << "size of vint: " << vint.size() << "\n";
    cout << "capacity of vint: " << vint.capacity() << "\n";
    return 0;
}
