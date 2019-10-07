/*************************************************************************
    > File Name: tst-lambda_implict_capture.cpp
    > Author: ma6174
    > Mail: ma6174@163.com
    > Created Time: Mon 18 Apr 2016 06:30:56 PM CST
 ************************************************************************/

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>

using namespace std;

void elimDups(vector<string> &words)
{
    sort(words.begin(), words.end());

    auto end_unique = unique(words.begin(), words.end());

    words.erase(end_unique, words.end());
}

void bigges(vector<string> &words,
            vector<string>::size_type sz,
            ostream &os = cout, char c = ' ')
{
    elimDups(words);

    stable_sort(words.begin(), words.end(),
    [](const string&a, const string&b) {
        return a.size() < b.size();
    });

    for_each(words.begin(), words.end(),
    [&, c](const string& s) {
        os << s << c;
    });

    cout << "\n";

    auto wc = find_if(words.cbegin(), words.cend(),
    [sz](const string& a) {
        return a.size() >= sz;
    });

    cout << *wc << "\n";

    for_each(words.begin(), words.end(),
    [&, c](const string &s) {
        os << s << c;
    });

    cout << "\n";

    for_each(words.begin(), words.end(),
    [=, &os](const string &s) {
        os << s << c;
    });

    cout << "\n";
}

int main()
{
    vector<string> svec{
        "vector", "string", "wangsr", "Apr", "local", "host", "abc", "string"
    };

    bigges(svec, 3);

    return 0;
}
