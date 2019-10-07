#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    string a("I'am a string.");

    std::vector<string> vstr, vstr1;
    vstr.push_back(std::move(a));
    std::cout << "string a.size after move " << a.size() << std::endl;

    vstr1 = std::move(vstr);
    std::cout << "vector of string vstr.size after move " << vstr.size() << std::endl;

    return 0;
}
