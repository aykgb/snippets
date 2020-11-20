#include <iostream>
#include <fstream>
#include <string>

using namespace std;

std::string path = "./tst";

int main() {
    std::ofstream fs(path.c_str(), ios::out);
    fs << "hello ofstream." << std::endl;
    fs << "hello ofstream." << std::endl;

    return 0;
}

