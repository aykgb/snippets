#include <iostream>
#include <stdlib.h>

using namespace std;

int main() {
    abort();

    /*  MacOS
        ulimit -a // 查看配置
        ulimit -c unlimited // 设置core文件大小为无限
        /core/core.pid // core文件位置
        lldb -c core.pid // 打开core
        bt // lldb 打印backtrace
    */

    return 0;
}