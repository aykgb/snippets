#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

int main() {
    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;

    std::thread consumer([&]{
        std::unique_lock<std::mutex> lock(mtx);
        while(!ready) {
            std::cout << " wait for 100ms ..." << std::endl;
            if(cv.wait_for(lock, std::chrono::milliseconds(100)) == std::cv_status::timeout) {
                std::cout << "timeout" << std::endl;
            } else {
                std::cout << "locked " << std::endl;
            }
        }
        std::cout << "ready " << ready << std::endl;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(110));
    ready = true;
    cv.notify_one();

    consumer.join();
}
