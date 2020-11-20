#include <iostream>
#include <vector>
#include <string>

#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;
namespace fs=boost::filesystem;

int main() {
    int version = 20;
    string prefix = "dict_file_";

    for (auto i = 0; i < version; i++) {
        auto fn = prefix + std::to_string(i);
        if(fs::exists(fn+".gz")) {
            fs::remove(fn+".gz");
            std::cout << fn+".gz" << " removed.\n";
        }
        if(fs::exists(fn+".lock")) {
            fs::remove(fn+".lock");
            std::cout << fn+".lock" << " removed.\n";
        }
        if(fs::exists(fn+".md5")) {
            fs::remove(fn+".md5");
            std::cout << fn+".md5" << " removed.\n";
        }
    }



    return 0;
}
