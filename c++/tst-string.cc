#include <iostream>
#include <string>

#include <stdio.h>

using namespace std;

int main() {
    std::string str("Hello String.");

    unsigned char *ptr = (unsigned char *)&str[0];

    printf("%s\n", ptr);

    std::string method = "func";

    std::cout << method.compare("func") << std::endl;
    std::cout << method.compare("func1") << std::endl;

    return 0;
}
