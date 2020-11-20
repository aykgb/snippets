#include <iostream>
#include <vector>

using namespace std;

int main ()
{
    std::vector<int> myvector (10);   // 10 zero-initialized ints

    // assign some values:
    for (unsigned i=0; i<myvector.size(); i++)
        myvector.at(i)=i;

    std::cout << "myvector contains:";
    for (unsigned i=0; i<myvector.size(); i++)
        std::cout << ' ' << myvector.at(i);
    std::cout << '\n';

    //  std::cout << myvector.at(10) << std::endl;
    myvector.at(10);

    return 0;
}
