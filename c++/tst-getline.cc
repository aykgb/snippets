#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main() {
    char buf[20];
    sprintf(buf, "%s", "aaa\nbbb\nccc\nddd\nee");

    memset(buf, 20, 0);
    std::istringstream stream(buf);

    string line;
    while (std::getline(stream, line)) {
        std::cout << line << "\n";
    }

    return 0;
}
