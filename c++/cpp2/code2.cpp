/*************************************************************************
    > File Name: code2.cpp
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: Wed 01 Jun 2016 04:44:24 PM CST
 ************************************************************************/

#include<iostream>
#include<string>
using namespace std;

int main()
{
    const float x = 1.1;
    const float z = 1.123;
    float y = x;
    for(int j = 0; j < 9000000; ++j) {
        y *= x;
        y /= z;
        y += 0;
        y -= 0;
    }
    return 0;
}
