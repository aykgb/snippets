#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <atomic>
#include <future>

using namespace std;

class TEST {
    bool stop = false;
    std::mutex mtx;
    std::condition_variable cv_monitor;
    std::condition_variable cv_job;

    uint64_t counter = 0;

  public:
    TEST() { }

    void AddJobs2() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        counter++;
        if (counter > 1000) {
            std::unique_lock<std::mutex> locker(mtx);
            std::cout << "launch AsyncMonitor.\n";
            std::async(std::launch::async, [this] {
                AsyncMonitor();
            });
            // 等10ms，表检查条件是否满足
            std::cout << "AddJobs waiting.\n";
            cv_job.wait(locker);
        }
    }

    void AddJobs() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        counter++;
        if (counter > 1000) {
            std::unique_lock<std::mutex> locker(mtx);
            std::cout << "AddJobs counter > 1000 and cv_monitor.notify\n";
            cv_monitor.notify_all();
            // 等10ms，表检查条件是否满足
            std::cout << "AddJobs waiting.\n";
            cv_job.wait(locker);
        }
    }

    void DecJobs() {
        while(!stop || counter > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            if(counter > 0) {
                counter--;
            }
        }

        std::cout << "DecJobs ends with counter " << counter << "\n";
    }

    void Monitor() {
        while(!stop) {
            std::unique_lock<std::mutex> locker(mtx);
            std::cout << "Monitor waiting...\n";
            cv_monitor.wait(locker);
            std::cout << "Monitor notified.\n";
            while(counter > 100);
            std::cout << "Monitor counter " << counter << " and cv_job.notify\n";
            cv_job.notify_all();
        }
    }

    void AsyncMonitor() {
        std::cout << "start async monitor.\n";
        std::unique_lock<std::mutex> locker(mtx);
        std::cout << "locker locked.\n";
        while(counter > 1000) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
        std::cout << "end async monitor cv_job.notify_all.\n";
        cv_job.notify_all();
    }

    void Stop() {
        std::cout << "stop.\n";
        stop = true;
        cv_monitor.notify_all();
    }
};

int main() {
    TEST tst;
    std::thread th2([&] {
        tst.Monitor();
    });

    std::thread th1([&] {
        tst.DecJobs();
    });
    for(int i = 0; i< 10000; i++) {
        tst.AddJobs2();
    }

    tst.Stop();
    th1.join();
    th2.join();

    return 0;
}
