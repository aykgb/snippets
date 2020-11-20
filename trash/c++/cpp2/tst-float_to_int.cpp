#include <iostream>

using namespace std;

int main()
{

    float tx_power = 1.1;
    int mdBm = static_cast<int>(tx_power * 100);
    std::cout << "#### ALS: mdBm:" << mdBm << std::endl;

    return 0;
}
