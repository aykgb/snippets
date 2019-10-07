#include <iostream>
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>

using namespace std;

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void do_print_id(int id)
{
    std::unique_lock<std::mutex> lck(mtx);
    std::cout << "function id: " << id
              << "is waiting for lck ..." << std::endl;

    while(!ready)
        cv.wait(lck);

    std::cout << "function id: " << id << " is finished." << std::endl;
}

void go()
{
    std::unique_lock<std::mutex> lck(mtx);
    ready = true;
    cv.notify_all();
}

int main()
{
    std::vector<std::thread> threads(10);

    for(int i = 0; i < 10; ++i) {
        threads[i] = std::thread(do_print_id, i);
    }

    std::cout << "10 threads ready to race ..." << std::endl;

    go();

    for(auto& th:threads)
        th.join();

    return 0;
}

