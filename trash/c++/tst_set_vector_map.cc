#include <map>
#include <set>
#include <vector>
#include <string>
#include <iostream>
#include <mutex>

using namespace std;


/**
 * 多机build在一台build server上存在一个table的
 * 多个分片列表build任务的情况下，需要从元数据文件名
 * 中恢复分片列表放到内存，可用于快速查询。
 *  */
std::mutex mtx_table_plist; // map tablename to partitions_list
std::map<string, std::set<int>> g_table_plists;
int SetTablePartsList(string assembled_tbname, const std::vector<int>& part_list) {
    std::lock_guard<std::mutex> lock(mtx_table_plist);
    g_table_plists[assembled_tbname] = std::set<int>(part_list.begin(), part_list.end());
    return 0;
}

int GetTablePartsList(string assembled_tbname, std::vector<int>& part_list) {
    std::lock_guard<std::mutex> lock(mtx_table_plist);
    auto find = g_table_plists.find(assembled_tbname);
    if(find == g_table_plists.end()) {
        return -1;
    }
    auto list = find->second;
    part_list = std::vector<int>(list.begin(), list.end());

    return 0;
}

int GetAssembledTableName(int pid, const string& tablename, string& assembled_tablename) {
    std::lock_guard<std::mutex> lock(mtx_table_plist);
    bool flag_find = false;
    for(auto &item : g_table_plists) {
        string assembled_tbname = item.first;
        std::cout << assembled_tbname << std::endl;
        int pos = assembled_tbname.find_last_of("_");
        string tbname = assembled_tbname.substr(0, pos);
        std::cout << tbname << std::endl;
        if (tbname == tablename) {
            auto find = item.second.find(pid);
            if(find != item.second.end()) {
                flag_find = true;
                assembled_tablename = assembled_tbname;
                break;
            }
        }
    }

    if(!flag_find) {
        return -1;
    }
    return 0;
}

int main() {
    SetTablePartsList("tableA_0", {1, 0, 2});
    SetTablePartsList("tableA_1", {5, 4, 3});

    std::vector<int> list;
    GetTablePartsList("tableA_1", list);

    for(auto pid : list) {
        std::cout << pid << " ";
    }
    std::cout << "\n";

    string assembled_tbname;
    GetAssembledTableName(0, "tableA", assembled_tbname);
    std::cout << "assembled_tbname:" << assembled_tbname << std::endl;

    return 0;
}
