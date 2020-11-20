#include <iostream>
#include <iomanip>
#include <map>
#include <list>
#include <iterator>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
// #include <boost/algorithm/string.hpp>

using namespace std;
// using namespace boost;

std::string& trim_right_inplace(
    std::string&       s,
    const std::string& delimiters = " \f\n\r\t\v" )
{
    return s.erase( s.find_last_not_of( delimiters ) + 1 );
}

std::string& trim_left_inplace(
    std::string&       s,
    const std::string& delimiters = " \f\n\r\t\v" )
{
    return s.erase( 0, s.find_first_not_of( delimiters ) );
}

std::string& trim_inplace(
    std::string&       s,
    const std::string& delimiters = " \f\n\r\t\v" )
{
    return trim_left_inplace( trim_right_inplace( s, delimiters ), delimiters );
}

int main()
{
    ifstream in("tst-words.txt");
    //  istream_iterator<string> is_it(in), eof;
    map<string, list<int> > map_word_linum;
    uint32_t linum = 0;
    while(!in.eof()) {
        string line;
        std::getline(in, line);
        stringstream ss(line);
        istream_iterator<string> is_it(ss), eof;
        vector<string> words(is_it, eof);
        /* trim ',.()' from a string. */
        for_each(words.begin(), words.end(),
        [&](string& word) {
            trim_inplace(word, ".,");
        });

        sort(words.begin(), words.end());
        auto it = unique(words.begin(), words.end());
        words.resize(std::distance(words.begin(), it));

        for_each(words.begin(), words.end(),
        [&](const string& word) {
            map_word_linum[word].push_back(linum);
        });

        linum++;

        ostream_iterator<string> os_it(cout, "|");
        cout << "-----------------------------------------\n";
        cout << "#### ";
        copy(words.begin(), words.end(), os_it);
        cout << endl;
    }
    cout << "\n";

    for(const auto& it : map_word_linum) {
        cout << "#### " << right << setw(15) << it.first;
        cout << ": ";
        ostream_iterator<int> os_it(cout, " ");
        copy(it.second.begin(), it.second.end(), os_it);
        cout << "\n";
    }
    cout << endl;

    //    unique_copy(line.begin(), line.end(), is_it);
    //    if(*is_it == "\n") ++linum;
    //    map_word_linum[*is_it++].pop_back()

    //}
    //


    return 0;
}
