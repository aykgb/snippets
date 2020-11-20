#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

int main() {
    // string trpv_name = "mytest_table_3_128_0";
    // string trpv_name = "mytest_table_3_128";
    string trpv_name = "tst_table_3_128";

    int pos_version = trpv_name.rfind('_');
    string str_version = trpv_name.substr(pos_version + 1);
    std::cout << pos_version << " " << str_version << std::endl;

    int pos_pid = trpv_name.rfind('_', pos_version - 1);
    string str_pid = trpv_name.substr(pos_pid + 1, pos_version - pos_pid - 1);
    std::cout << pos_pid << " " << str_pid << std::endl;

    int pos_rid = trpv_name.rfind('_', pos_pid - 1);
    string str_rid = trpv_name.substr(pos_rid + 1, pos_pid - pos_rid - 1);
    std::cout << pos_rid << " " << str_rid << std::endl;

    string tablename = trpv_name.substr(0, pos_rid);
    int res = atoi(tablename.c_str());
    std::cout << tablename << " to int:" << res << std::endl;

    return 0;
}
