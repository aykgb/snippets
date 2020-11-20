#include <iostream>
#include <chrono>
#include <unordered_map>
#include <map>

using namespace std;

struct MockDB {
    MockDB(int index):id(index) {}
    void set_id(int p) {
        id = p;
    }
    int id;
};

template<typename TimeT = std::chrono::milliseconds>
struct measure {
    template<typename F, typename ...Args>
    static typename TimeT::rep execution(F&& func, Args&&... args) {
        auto start = std::chrono::steady_clock::now();
        std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
        auto duration = std::chrono::duration_cast< TimeT>
                        (std::chrono::steady_clock::now() - start);
        return duration.count();
    }
};


int main() {
    std::map<int, MockDB*> dbs_;
    // std::unordered_map<int, MockDB*> dbs_;
    // dbs_.reserve(1024);
    // dbs_.max_load_factor(0.25);
    dbs_.clear();

    auto func = [&]() {
        for(int t = 0; t < 10000; t++) {
            for(int i = 0; i < 128; i++) {
                dbs_[i] = new MockDB(i);
            }

            for(int i = 0; i < 128; i++) {
                auto it = dbs_.find(i);
                if (it != dbs_.end()) {
                    it->second->set_id(357);
                }
            }
        }

    };

    std::cout << measure<>::execution(func) << std::endl;

    return 0;
}
