#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <glog/logging.h>

using namespace google;

int task() {
    int count = 0;
    while(true) {
        count++;
        LOG(INFO) << "task doing... " << count << " times";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}

class Thread {
    public:
        Thread(std::function<int()> task): worker_(task) {};
        ~Thread() { LOG(INFO) << "=====> thread destructor called"; }

        void Detach() {
            worker_.detach();
            LOG(INFO) << "====> thread detached";
        }

        bool Joinable() {
            return worker_.joinable();
        }

        void Join() {
            worker_.join();
        }

    private:
        std::thread worker_;
};

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    // google::InitGoogleLogging(argv[0]);

    auto th = std::unique_ptr<Thread>(new Thread(task));
    std::this_thread::sleep_for(std::chrono::milliseconds(201));
    th->Detach();

    std::this_thread::sleep_for(std::chrono::milliseconds(1001));
    th = nullptr;
    LOG(INFO) << "wait for 1001 ms...";
    std::this_thread::sleep_for(std::chrono::milliseconds(1001));
    return 0;
}
