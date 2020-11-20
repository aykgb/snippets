#include <iostream>
#include <string>

using namespace std;

int main() {
    string strs = "aaaaaaaaaa";
    cout << strs.find_last_of('\t') << "\n";
    string strs1 = "aaaa\tbbbb";
    std::cout << strs1.find_last_of('\t') << "\n";
    string strs2 = "aaaa\tbbbb\tcccc";
    std::cout << strs2.find_last_of('\t') << "\n";
    string strs3 = "\taaaa\tbbbb\tcccc";
    std::cout << strs3.find_last_of('\t') << "\n";
    string strs4 = "cccc\t";
    std::cout << strs4.find_last_of('\t') << "\n";
    string strs5 = "aaaa\tbbbbb";
    size_t pos = strs5.find_last_of('\t');
    std::cout << "-------\n";
    std::cout << strs5.substr(0, pos) << "\n";
    std::cout << strs5.substr(pos+1) << "\n";
    pos = strs5.find_last_of(',');
    if(pos == std::string::npos) {
        std::cout << "','not found.\n";
    }


    return 0;
}
