#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include <map>

using namespace std;

int main() {
    std::map<int, std::shared_ptr<string>> key2string;

    std::cout << "Before size: " << key2string.size() << std::endl;

    std::shared_ptr<string>& sptr = key2string[0];
    if(sptr == nullptr) {
        std::cout << "nullptr." << std::endl;
    } else {
        std::cout << "other." << std::endl;
    }

    std::cout << "After size: " << key2string.size() << std::endl;

    return 0;
}
