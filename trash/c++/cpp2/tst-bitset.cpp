#include <iostream>
#include <string>
#include <bitset>

using namespace std;

int main()
{
    std::bitset<12> bset0("100011110001");
    bset0 >>= 1;
    bset0 <<= 1;
    cout << bset0 << endl;

    bset0.set(0);
    cout << bset0.to_string() << endl;
    cout << (bset0.to_ulong() & 0xfffffffe) << endl;

    return 0;
}
