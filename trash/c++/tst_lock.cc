#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>
#include <vector>
#include <condition_variable>
#include <memory>

std::mutex mtx_;
std::condition_variable cv_;

int64_t in_flight_num_ = 0;
int64_t max_ = 1024;
bool stop_ = false;

void put() {
    while(!stop_) {
        std::unique_lock<std::mutex> guard(mtx_);
        while(in_flight_num_ >= max_) {
            cv_.wait_for(guard, std::chrono::milliseconds(50));
            if(stop_) {
                break;
            }
        }
        in_flight_num_++;
        guard.unlock();
    }
    std::cout << "put exit...\n";
}

void done() {
    while(!stop_) {
        std::unique_lock<std::mutex> guard(mtx_);
        // TODO:do something
        in_flight_num_--;
        guard.unlock();
        cv_.notify_all();
    }
    std::cout << "done exit...\n";
}

int main() {
    std::vector<std::thread> workers_put;
    std::vector<std::thread> workers_done;
    for(auto i = 0; i < 100; i++) {
        workers_put.emplace_back(put);
    }
    for(auto i = 0; i < 6; i++) {
        workers_done.emplace_back(done);
    }

    for(int i = 0; i < 30;i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "doing...\n";
    }
    std::cout << "stop and wait...\n";
    stop_ = true;

    for(auto i = 0; i < 100; i++) {
        if(workers_put[i].joinable()) {
            workers_put[i].join();
        }
    }
    for(auto i = 0; i < 6; i++) {
        if(workers_done[i].joinable()) {
            workers_done[i].join();
        }
    }

    std::cout << "all done\n";

    return 0;
}
