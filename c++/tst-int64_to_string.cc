#include <iostream>
#include <string>

using namespace std;

int main() {
    int64_t id = 12345678;
    int size = sizeof(id);
    char buffer[size + 1];
    snprintf(buffer, size + 1, "%lld", id);
    std::string id_str(buffer);
    std::cout << id_str << std::endl;

    return 0;
}
