#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <memory>
#include <algorithm>

using namespace std;
using line_no = std::vector<std::string>::size_type;

class TextQuery;
class QueryResult;

std::ostream& print(std::ostream&, const QueryResult&);

class QueryResult {
    friend std::ostream& print(std::ostream&, const QueryResult&);
public:
    QueryResult(std::string s,
                std::shared_ptr<std::set<line_no>> p,
                std::shared_ptr<std::vector<std::string>> f):
        sought(s), lines(p), file(f) { }
private:
    // share_ptr<>
    std::string sought; // 查询的的单词
    std::shared_ptr<std::set<line_no>> lines;
    std::shared_ptr<std::vector<std::string>> file;
};


/* 'word' 'line_num' 'line'*/
std::ostream& print(std::ostream& os, const QueryResult& qr)
{
    // 如果找到单词，打印出现的次数和所有出现的位置
    os << qr.sought << " occurs " << qr.lines->size() << " " << endl;
    for(auto num: *qr.lines)
        os << "\t(line " << num + 1 << ") " << *(qr.file->begin() + num) << endl;

    return os; // 返回os，是将os返回操作系统
}

class TextQuery {
public:
    //  TextQuery();
    TextQuery(std::ifstream&);
    // ~TextQuery();
    QueryResult query(const std::string&) const;

    void print();

private:
    shared_ptr<vector<string>>             file;
    map<string, shared_ptr<set<line_no>>>  word_map;
};

TextQuery::TextQuery(std::ifstream& is): file(new vector<string>)
{
    std::string text;
    std::istringstream iss;
    while(std::getline(is, text)) { // 对此文件中的每行
        file->push_back(text);      // 保存此行文本
        int n = file->size() - 1;   // 当前行好
        istringstream line(text);   // 将文本分解为单词
        string word;
        while(line >> word) {       // 对行中的每个单词
            auto &lines = word_map[word];    // 如果单词在map中，读取set，否则新建一项
            if(!lines)
                lines.reset(new set<line_no>); // 分配一个新的set
            lines->insert(n);                // 将此行号插入set
        }
    }
    print();
}

void TextQuery::print()
{
    std::cout << "\n#### text context ####" << std::endl;
    int n = 0;
    for_each(file->begin(), file->end(),
    [&](const string &it) {
        cout << n << " "<<  it << endl;
        n++;
    });

    std::cout << "#### context end ####\n" << std::endl;
}

QueryResult TextQuery::query(const std::string& sought) const
{
    static shared_ptr<set<line_no>> nodata(new set<line_no>);
    auto loc = word_map.find(sought);
    if(loc == word_map.end())
        return QueryResult(sought, nodata, file); //未找到
    else
        return QueryResult(sought, loc->second, file);
}

void runQueries(ifstream& ifile)
{
    TextQuery tq(ifile);
    while(true) {
        cout << "enter word to look for, or q to quit: ";
        string s;
        if(!(cin >> s) || s == "q") break;
        print(cout, tq.query(s)) << endl;
    }
}

int main()
{
    string filename;
    cout << "Enter the file name: ";
    cin >> filename;

    ifstream infile(filename);

    runQueries(infile);
    return 0;
}
