#include <iostream>
#include <string>

using namespace std;

int main()
{
    string str1 = "12345678";
    string str2 = "88769874";
    string str3 = "";

    char a = str1[0] + str2[1] - '0';

    // str3.pop_back();
    cout << a << endl;
    int slot = 0;
    static std::string outfilename = "hal_debug_slot";
    std::string filename = outfilename +static_cast<char>('0' + slot) + ".log";

    cout << filename << endl;

    return 0;
}
