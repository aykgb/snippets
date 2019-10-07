/*************************************************************************
    > File Name: tst-ifstream_eof.cpp
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: Tue 19 Apr 2016 02:40:39 PM CST
 ************************************************************************/

#include<iostream>
#include <fstream>      // std::ifstream

int main ()
{

    std::ifstream is("example.txt");   // open file

    char c;
    while (is.get(c))                  // loop getting single characters
        std::cout << c;

    if (is.eof())                      // check for EOF
        std::cout << "[EoF reached]\n";
    else
        std::cout << "[error reading]\n";

    is.close();                        // close file

    return 0;
}
