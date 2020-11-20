#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>

volatile int sink;
int main()
{
    auto start = std::chrono::steady_clock::now();
    auto epoch = start.time_since_epoch();
    std::cout << "time since epoch:" <<
      std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count() << std::endl;
}
