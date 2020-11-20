#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iterator>

#include <string.h>

using namespace std;

map<string, string> buildMap(ifstream& map_file)
{
    map<string, string> trans_map;
    string key;
    string value;
    /* 读取第一个单词， 从行中剩余的内容读取value。 */
    while(map_file >> key && getline(map_file, value)) {
        if(value.size() > 1) { // 包含空格字符
            trans_map[key] = value.substr(1);  // 跳过前导空格字符
        } else
            throw runtime_error("no rule for key " + key);
    }
    return trans_map;
}

const string& transform(const string &s, const map<string, string> &m)
{
    auto map_it = m.find(s);
    if(map_it != m.cend())
        return map_it->second;
    else
        return s;
}

void word_transform(ifstream& map_file, ifstream& input)
{
    /* 读取map_file文件 */
    auto trans_map = buildMap(map_file);
    string text;
    while(getline(input, text)) {
        istringstream stream(text);
        string word;
        bool firstword = true;
        while(stream >> word) {
            if(firstword)
                firstword = false;
            else
                cout << " ";
            cout << transform(word, trans_map);
        }
        cout << endl;
    }
}

int main(int argc, char *argv[])
{
    cout << argc << "\n";
    cout << argv[0] << "\n";
    // /* Must check the argc first, in case that invalid access memary fault occur. */
    // if(argc > 1 && strncmp(argv[1], "tst", 3) == 0)
    //   cout << argv[1] << "\n";



    return 0;
}
