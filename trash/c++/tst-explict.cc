#include <iostream>
#include <string>
#include <memory>

using namespace std;

class Base {
public:
    explicit Base(std::shared_ptr<string> data):data_(data) {}
    ~Base() {
        std::cout << data_.get()->data() << std::endl;
    }

private:
    std::shared_ptr<string> data_;
};

int main() {
    string s0 = "12345";
    auto s1 = std::make_shared<string>("123456");
    const string &s2 = s0;

    // Base b0(s0);
    Base b1(s1);
    // Base b2(s2);

    return 0;
}
