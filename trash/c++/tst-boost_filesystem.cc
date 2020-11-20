#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/format.hpp>

using namespace std;
using namespace boost::filesystem;

namespace fs = boost::filesystem;

int ListAllTableParts(const string& db_dir, const string& table_name,
                      std::vector<string>& table_parts) {
    namespace fs = boost::filesystem;
    if (!fs::exists(fs::path(db_dir)) || !fs::is_directory(db_dir)) {
        std::cout << db_dir << " is not a directory or not existing.\n";
        return -1;
    }
    fs::directory_iterator end_iter;
    for (fs::directory_iterator iter_dir(db_dir); iter_dir != end_iter;
         iter_dir++) {
        string dir_name = iter_dir->path().string();
        if (!fs::is_directory(*iter_dir)) {
            continue;
        }
        std::cout << dir_name << "\n";
        size_t pos = dir_name.find(table_name);
        if (pos != string::npos) {
            string table_part = dir_name.substr(pos);
            std::cout << "== find table " << table_name << " " << table_part
                      << "\n";
            table_parts.push_back(table_part);
        }
    }

    return 0;
}

int main() {
#if 0
    // test case 0 : pass
    std::string cwd = fs::current_path().generic_string();

    std::string from = "./old_dir";
    std::string to = "./new_dir";
    // test case 1: fail
    std::string cwd = fs::current_path().generic_string();

    std::string from = cwd + "/new_dir";
    std::string to = cwd + "/new_dir/new_dir";
#endif
    // test case 2: pass
    // fs::create_directory("./new_dir");
    // fs::create_directory("./new_dir");
    // std::string from = "./old_dir";
    // std::string to = "./new_dir/new_dir";

    // try {
    //     fs::rename(fs::path(from), fs::path(to));
    // } catch (std::exception& e) {
    //     std::cout << e.what() << std::endl;
    // }
    std::vector<string> table_parts;
    ListAllTableParts("db_path", "table_a", table_parts);

    return 0;
}
