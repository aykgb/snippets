/*************************************************************************
    > File Name: tst-stl_algorithm_unique.cpp
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: Mon 18 Apr 2016 06:10:44 PM CST
 ************************************************************************/

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>

using namespace std;

int main()
{
    vector<string> vstr{
        "fox", "jumps", "over", "quick", "red", "red", "slow", "the", "the", "turtle"
    };

    auto end_unique = std::unique(vstr.begin(), vstr.end());
    cout << *end_unique << endl;

    return 0;
}

