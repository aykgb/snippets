#include <functional>
#include <iostream>

using namespace std;

int add2num(int a, int b) { return a + b; }

struct default_filter {
    typedef bool result_type;
    result_type operator()(string const&) const { return false; }
};

int main() {
    std::function<int(int)> tst = std::bind(add2num, 1, std::placeholders::_1);
    int res = tst(2);
    std::cout << res << std::endl;

    std::string strs = "null";
    std::function<bool(string)> filter{default_filter()};
    std::cout << filter(strs) << std::endl;

    return 0;
}
