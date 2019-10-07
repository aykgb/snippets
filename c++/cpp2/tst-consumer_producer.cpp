#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

using namespace std;

std::mutex mtx;
std::condition_variable consume, produce;

std::queue<int> task;
const int MAX_QUEUE_SIZE = 20;

void consumer()
{
    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::unique_lock<std::mutex> lck(mtx);
        consume.wait(lck, []() {
            return task.size() != 0;
        });

        std::cout << "consumer " << std::this_thread::get_id() << " :";
        task.pop();
        std::cout << task.size() << "\n";

        produce.notify_all();
    }
}

void producer(int id)
{
    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::unique_lock<std::mutex> lck(mtx);
        produce.wait(lck, []() {
            return task.size() != MAX_QUEUE_SIZE;
        });

        std::cout << "producer " << std::this_thread::get_id() << " :";
        task.push(id);
        std::cout << task.size() << "\n";

        consume.notify_all();
    }
}

int main()
{
    std::thread consumers[10], producers[10];
    for(int i = 0; i < 10; ++i) {
        consumers[i] = std::thread(consumer);
        producers[i] = std::thread(producer, i + 1);
    }

    for(int i = 0; i < 10; ++i) {
        consumers[i].join();
        producers[i].join();
    }

    return 0;
}
