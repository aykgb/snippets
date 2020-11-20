#include <iostream>
#include <chrono>
#include <string>

int main() {
    auto now = std::chrono::steady_clock::now().time_since_epoch();
    auto elapse_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now);
    auto elapse_s = std::chrono::duration_cast<std::chrono::seconds>(now);
    std::cout << "time since eppch " << elapse_ms.count() << " ms" << std::endl;
    std::cout << "time since eppch " << elapse_s.count() << " s" << std::endl;

    now = std::chrono::system_clock::now().time_since_epoch();
    elapse_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now);
    elapse_s = std::chrono::duration_cast<std::chrono::seconds>(now);
    std::cout << "time since eppch " << elapse_ms.count() << " ms" << std::endl;
    std::cout << "time since eppch " << elapse_s.count() << " s" << std::endl;
}
