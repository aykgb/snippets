#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

using namespace std;

int64_t GetRocksDBAllSstSize(const string& db_path) {
    namespace fs = boost::filesystem;
    fs::path fullpath(db_path);

    if (!fs::exists(fullpath)) {
        std::cout << fullpath << " not exists.\n";
        return -1;
    }

    int64_t sst_size_total = 0;
    fs::directory_iterator end_iter;
    for (fs::directory_iterator iter(fullpath); iter != end_iter; iter++) {
        try {
            if (fs::is_regular_file(*iter) && fs::extension(*iter) == ".sst") {
                sst_size_total += fs::file_size(*iter);
            }
        } catch (std::exception& e) {
            std::cout << e.what() << "\n";
            return -2;
        }
    }
    return sst_size_total;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "empty path.\n";
        return -1;
    }

    string dbpath(argv[1]);

    std::cout << "file " << dbpath << " size is "
              << GetRocksDBAllSstSize(dbpath) << "\n";
    return 0;
}
