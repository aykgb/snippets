#include <iostream>
#include <string>
#include <cstdio>

using namespace std;

int main() {
    string old_dir = "./old_dir";
    string new_dir = "./dst_dir/new_dir";

    char cwd[128];
    int r = rename(old_dir.c_str(), new_dir.c_str());

    if(r != 0) {
        std::cout << "move failed.\n";
    }

    return 0;
}
