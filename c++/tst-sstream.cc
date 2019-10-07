#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main() {
    int64_t id = 12345678;
    std::string id_str;
    std::stringstream ss;
    ss << id;
    ss >> id_str;

}
