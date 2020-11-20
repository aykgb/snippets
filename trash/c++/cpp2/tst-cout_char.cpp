#include <iostream>
#include <string>

using namespace std;

int main()
{
    char a = 0x04;
    cout << "0x" << hex << static_cast<int>(a) << endl;
    return 0;
}
