/*************************************************************************
    > File Name: tst-stl_algorithm_equal.cpp
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: Mon 18 Apr 2016 05:49:00 PM CST
 ************************************************************************/

#include<iostream>
#include<vector>
#include<list>

using namespace std;

int main()
{
    vector<const char*> roster1{"a", "ab", "abc"};

    list<const char*> roster2{
        "a",
        "ab",
        "abc"
    };

    std::cout << std::equal(roster1.cbegin(), roster1.cend(), roster2.cbegin()) << "\n";

    return 0;
}
