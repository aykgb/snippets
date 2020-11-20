#include <iostream>
#include <chrono>

int main()
{
    std::chrono::milliseconds dura(100);
    std::cout << dura.count() << std::endl;
    auto dura_us = std::chrono::duration_cast<std::chrono::microseconds>(dura);
    std::cout << dura_us.count() << std::endl;

    auto func = [](std::chrono::microseconds us) {
        std::cout << us.count() << std::endl;
    };
    func(dura);
}
