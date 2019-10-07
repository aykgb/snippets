#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename R, typename ... Ts>
auto count(const R& range, Ts ... ts) {
    return (std::count(std::begin(range), std::end(range), ts) + ...);
}

int main() {

    std::vector<int> intVec {1, 2, 3, 4, 5, 6, 7, 11, 13};
    std::cout << count(intVec, 1, 2) << std::endl;

    return 0;
}
