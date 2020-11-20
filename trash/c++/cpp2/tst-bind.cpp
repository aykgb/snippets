/*************************************************************************
    > File Name: tst-bind.cpp
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: Mon 18 Apr 2016 08:46:38 PM CST
 ************************************************************************/

#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<functional>
#include<algorithm>
#include<boost/algorithm/string.hpp>

using namespace std;
using namespace std::placeholders;
using namespace boost::algorithm;

bool check_size(const string &s, string::size_type sz, uint32_t& cnt)
{
    if(s.size() <= sz) {
        cnt++;
        return true;
    }
    return false;
}

int main()
{
    ifstream ifile("./words.txt");
    stringstream ss;
    string line;
    vector<string> words;
    uint32_t cnt = 0;
    while(!ifile.eof()) {
        std::getline(ifile, line);
        auto check6 = std::bind(check_size, _1, 6, ref(cnt));
        boost::algorithm::split(words, line, is_any_of(" "), token_compress_on);
        std::for_each(words.begin(), words.end(), check6);
    }
    std::cout << cnt << "\n";
    //	for_each()
    return 0;
}
