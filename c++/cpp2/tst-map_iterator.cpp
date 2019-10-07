#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <string>

using namespace std;

int main()
{
    map<vector<int>::iterator, int> map_vec_iter_int;
    map<list<int>::iterator, int> map_lis_iter_int;

    vector<int> ivec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    list<int> ilis = {0, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    for(const auto item : ivec) {
        cout << item << " | ";
    }
    cout << endl;

    vector<int>::iterator vec_it = ivec.begin();
    for(const auto item: ivec) {
        map_vec_iter_int.emplace(vec_it++, item);
    }

    // list的迭代器没有定义比较操作
    // list<int>::iterator lis_it = ilis.begin();
    // for(const auto item: ilis) {
    //   map_lis_iter_int.emplace(lis_it++, item);
    // }

    return 0;
}
