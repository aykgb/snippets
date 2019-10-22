#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

int main() {
    string s = "我爱你";

    printf("%s len(s):%d\n", s.c_str(), s.size());
    for(auto &e: s) {
        printf("%d ", e);
    }
    printf("\n");

    return 0;
}