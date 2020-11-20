#include <iostream>
#include <bitset>
#include <string>

using namespace std;

int main()
{

//  std::bitset<32> bitvec(0xa);
    int len = 100;
    std::bitset<32> b(0xa);
    cout.width(4);
    cout << hex << 0xa << dec << endl;
    cout << b << endl;
    b.set(32);


    return 0;
}
