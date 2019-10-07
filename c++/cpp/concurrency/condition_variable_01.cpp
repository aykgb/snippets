#include <iostream>
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>

using namespace std;

std::mutex mtx;
std::condition_variable cv;

void do_print_id(int id)
{
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck);
    std::cout << "function id: " << id << " is finished." << std::endl;
}

int main()
{
    std::vector<std::thread> threads(10);

    for(int i = 0; i < 10; ++i) {
        threads[i] = std::thread(do_print_id, i);
    }

    std::cout << "Wait for 3 seconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Ready go." << std::endl;
    cv.notify_all();

    for(auto& th:threads) {
        th.join();
    }

    return 0;
}

