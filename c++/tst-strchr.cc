#include <string.h>
#include <string>
#include <iostream>

using namespace std;

int main() {

    const char * orig = "hello\tworld";
    size_t length = 12;

    char *pcur = strchr(const_cast<char*>(orig), '\t');

    std::cout << pcur-orig+1 << std::endl;
    int key_len = pcur - orig;
    std::cout << "key:" << string(orig, key_len) << " "
                 "value:"  << string(pcur+1, length- key_len -1) << "\n";

    return 0;
}