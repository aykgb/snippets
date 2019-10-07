#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <unistd.h>

using namespace std;

int main() {
    string res;

    FILE *pp = popen("gunzip -c gesake_value_model.gz", "r");
    if (!pp) {
        printf("popen error :%s", strerror(errno));
        res = "fork or pipe fail";
        return -1;
    }

    std::cout << "step 0\n";
    char tmp[1024];  //设置一个合适的长度，以存储每一行输出
    while (fgets(tmp, sizeof(tmp), pp) != NULL) {
        // if (tmp[strlen(tmp) - 1] == '\n') {
        //     tmp[strlen(tmp) - 1] = '\0'; //去除换行符
        // }
        printf("%s", tmp);
        std::cout << "step 01\n";
    }
    std::cout << "step 1\n";

    pclose(pp);

    return 0;
}
