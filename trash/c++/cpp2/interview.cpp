#include <iostream>

using namespace std;

int tst000()
{
    std::cout << "Input the data: " << std::endl;
    int n = 0, m = -1;
    if(!(std::cin >> n >> m)) {
        std::cout << "error 1. " << std::endl;
        return -1;
    }
    if(!(n > 0 && n <= 100000000) && (m > 0 && m <= 100000)) {
        std::cout << "error 2. " << std::endl;
        return -1;
    }
    int dc = 0, dp = 0, hc = 0, hp = 0;
    for(int i = 0; i < m; ++i) {
        std::cin >> dc >> hc;
        if(!dp) {
            dp = dc;
            hp = hc;
        } else if(dc > dp) {
            int d = (hc - hp) / (dc - dp);
            if(d >= 0 && d <= 1) {
                hp = hc;
                dp = dc;
            } else {
                std::cout << "IMPOSIBLE" << std::endl;
                return -1;
            }
        } else {
            std::cout << "wrong input." << std::endl;
            return -1;
        }
    }
    std::cout << hp << std::endl;

    return 0;
}

int tst001()
{

    return 0;
}

int main()
{
    return 0;
}
