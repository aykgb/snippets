#include <iostream>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

int main()
{
    map<string, size_t> word_count;
    string word;
    while(cin >> word) {
        if(word != "0")
            ++word_count.insert({word, 0}).first->second;
        else
            break;
    }

    for_each(word_count.begin(), word_count.end(),
    [](const pair<string, size_t>& w) {
        cout << w.first << ": " << w.second << "\n";
    });

    cout << endl;

    return 0;
}
