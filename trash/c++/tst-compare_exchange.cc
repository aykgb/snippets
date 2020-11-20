#include <iostream>
#include <atomic>
#include <thread>

using namespace std;

std::atomic<int> state_;

int main() {
    state_ = 0;
    state_.compare_exchange_strong(1, 0);

    return 0;
}
