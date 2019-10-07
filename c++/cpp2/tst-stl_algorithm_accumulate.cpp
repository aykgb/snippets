#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
    vector<double> vdou{0.1, 1.2, 10.8, 2.22};

    double sum = std::accumulate(vdou.cbegin(), vdou.cend(), 0.0);

    cout << "sum: " << sum << "\n";

    return 0;
}
