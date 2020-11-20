#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

using namespace std;

std::atomic<bool> ready(false);
std::atomic_flag winner = ATOMIC_FLAG_INIT;

void count1m(int id)
{
    while(!ready) {
        std::this_thread::yield();
    }

    for(int i = 0; i < 1000000; ++i) { } ;

    if(!winner.test_and_set()) {
        std::cout << "Thread #" << id << " win.\n";
    }
}

int tst_000()
{
    std::vector<std::thread> threads_count1m;
    std::cout << "Spawning 10 threads that count to 1 million.\n";

    for(int i = 0; i < 10; ++i)
        threads_count1m.push_back(std::thread(count1m, i + 1));

    ready = true;

    for(auto &th : threads_count1m)
        th.join();

    return 0;
}

#include <sstream>
std::atomic_flag lock_stream = ATOMIC_FLAG_INIT;
std::stringstream number_stream;

void append_number(int x)
{
    while(lock_stream.test_and_set()) { }

    number_stream << "Thread #" << x << "\n";

    lock_stream.clear();
}

int tst_001()
{
    std::vector<std::thread> threads_append_number;

    for(int i = 0; i < 10; ++i)
        threads_append_number.push_back(std::thread(append_number, i + 1));

    for(auto &th : threads_append_number)
        th.join();

    std::cout << number_stream.str();

    return 0;
}

std::atomic_flag lock = ATOMIC_FLAG_INIT;
void f(int n )
{
    for(int cnt = 0; cnt < 100; ++cnt) {
        while(lock.test_and_set(std::memory_order_acquire))
            ; // spin
        std::cout << "Thread #" << n << "\n";

        lock.clear(std::memory_order_release);
    }

}

int tst_002()
{
    std::vector<std::thread> v;

    for(int i = 0; i < 10; ++i)
        v.emplace_back(f, i);

    for(auto &t : v)
        t.join();

    return 0;
}

int main()
{
    tst_002();
    return 0;
}
