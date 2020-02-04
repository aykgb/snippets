#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

void schedule() {
    while(true) {
        std::cout << "this thread shall sleep for 30 seconds...\n";
        std::this_thread::sleep_for(std::chrono::seconds(30));
        std::cout << "done";
    }
}

int main(int argc, char* argv[]) {
    std::thread worker(schedule);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    worker.~thread();
    if(worker.joinable()) {
        worker.join();
    }
    return 0;
}
