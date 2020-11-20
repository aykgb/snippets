/*************************************************************************
    > File Name: tst-istream_iterator.cpp
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: Wed 20 Apr 2016 09:13:48 AM CST
 ************************************************************************/

#include<iostream>
#include<fstream>
#include<iterator>
#include<algorithm>

using namespace std;

int main()
{
    ifstream in("./tst-words.txt", std::ifstream::in);
    istream_iterator<string> eof;
    istream_iterator<string> is_it(in);

    vector<string> svec;
    //  std::copy(svec.begin(), svec.end(), *is_it++);
    while(is_it != eof)
        //    cout << *is_it++ << "\n";
        svec.push_back(*is_it++);

    ostream_iterator<string> os_it(cout, " ");
    std::copy(svec.begin(), svec.end(), os_it);

    cout << "\n";

    return 0;
}
