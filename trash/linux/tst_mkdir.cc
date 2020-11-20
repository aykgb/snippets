#include <sys/stat.h>
#include <string>
#include <iostream>    

int main() {
    std::string dir = "/tmp/";
    auto ret = mkdir(dir.c_str(), 0755);
    std::cout << ret << "\n";
    std::cout << errno << "\n";
}
