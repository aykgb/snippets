#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> process_c11()
{
    return {"This is a string!", "This is a string!!", "This is a string!!!"};
}

int main()
{
    vector<string> vec_str = process_c11();
    cout << vec_str[0] << endl;

    return 0;
}