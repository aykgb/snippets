#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    vector<string> res(10);

    for(vector<string>::size_type i = 0; i < res.size(); ++i) {
        std::cout << res[i].size() << " ";
        std::cout << res[i].capacity() << " ";
        std::cout << res[i][2] + '0' << std::endl;
    }

    return 0;
}
