#include <string>
#include <iostream>

using namespace std;

int main() {
    string dirname0 = "/home/xiaoju/dc_build/data/db_data/tableA_0";
    string dirname1 = "/home/xiaoju/dc_build/data/db_data/tableA_1";

    string tbname = "tableA";

    int pos = dirname0.find_last_of("/");
    std::cout << dirname0.substr(pos + 1) << "\n";

    return 0;
}
