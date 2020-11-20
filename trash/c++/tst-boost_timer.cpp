#include <iostream>
#include <vector>
#include <string>
#include <boost/timer.hpp>
#include <chrono>
#include <thread>

using namespace std;
using namespace boost;

int main() {
    auto tp0 = std::chrono::steady_clock().now();
    for(auto i = 0; i< 100000; i++) { }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto tp1 = std::chrono::steady_clock().now();

    auto du = tp1 - tp0;
    std::cout << du.count() / CLOCKS_PER_SEC << std::endl;
}
