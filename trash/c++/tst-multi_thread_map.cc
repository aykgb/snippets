#include <thread>
#include <map>
#include <string>
#include <iostream>
#include <vector>

#include <condition_variable>
#include <mutex>

using namespace std;

std::map<int, string> map_key_value {
    {1, "wangchenglin"},
    {2, "age"},
    {3, "gender"},
    {4, "hobby"},
    {5, "height"},
    {6, "weight"}
};


std::condition_variable cv;
std::mutex mtx;

string value;

void init_map() {
    for(auto i = 0; i < 1024; i++) {
        map_key_value[i] = std::to_string(i);
    }
}

void read_map(string id) {
    std::cout << "thread " << id <<std::endl;
    while(true) {
        // auto iter = map_key_value.find(key);
        // if(iter != map_key_value.end()) {
        //      auto value = map_key_value[key];
        //      std::cout << "id " << id << " k " << key << " v " << value << "-->";
        // }
        // key++;
        // if(key >= 1024) {
        //     key = 1;
        // }
        value = map_key_value[1+std::stol(id)];
    }
}

void write_map(string id) {
    std::cout << "thread " << id <<std::endl;
    int value = 1;
    while(true) {
        // int key = 7;
        // key++;
        // auto value = std::to_string(key += 1010);
        // map_key_value[key] = value;
        // std::cout << "id " << id << " k " << key << " v " <<  value << "-->";
        map_key_value[value+std::stol(id)] = std::to_string(value);
        value++;
    }
}

int main() {
    std::thread r1 = std::thread(read_map, "100");
    std::thread w1 = std::thread(write_map, "201");
    std::thread w2 = std::thread(write_map, "202");
    std::thread w3 = std::thread(write_map, "203");
    std::thread insight([] {
        while(true) {
            std::cout << "value:" << value << "\n";
        }
    });

    std::vector<std::thread> read_threads;
    std::vector<std::thread> write_threads;

    read_threads.emplace_back(std::thread(read_map, "r" + std::to_string(0)));
    for(auto i = 0; i < 3; i++) {
        write_threads.emplace_back(std::thread(write_map, "w" + std::to_string(i)));
    }

    for(auto i = 0; i < 3; i++) {
        write_threads[i].join();
    }
    read_threads[0].join();

    insight.join();

    return 0;
}

