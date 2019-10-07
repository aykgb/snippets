#include <iostream>
#include <vector>
#include <string>

using namespace std;

constexpr static uint16_t HAS_KEY_ = 0b0001;
constexpr static uint16_t HAS_TTL_ = 0b0010;
constexpr static uint16_t HAS_FVS_ = 0b0100;
constexpr static uint16_t HAS_ALL_ = 0b0111;

uint16_t parse_flag_;
bool IsValid() {
    return parse_flag_ == HAS_ALL_;
}

void dump() {
    std::cout << "valid:" << IsValid() << "\n";
    std::cout << "parse_flag_:" << parse_flag_ << "\n";
}


int main() {
    parse_flag_ |= HAS_FVS_;
    dump();
    parse_flag_ |= HAS_KEY_;
    dump();
    parse_flag_ |= HAS_TTL_;
    dump();
    parse_flag_ &= ~HAS_TTL_;
    dump();

    return 0;
}
