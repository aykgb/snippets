#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void do_print_id(int id)
{
    std::unique_lock<std::mutex> lck(mtx);
    while(!ready)
        cv.wait(lck);
    std::cout << "id: " << id << std::endl;
}

void go()
{
    std::unique_lock<std::mutex> lck(mtx);
    ready = true;
    cv.notify_all(); // 唤醒所有线程
}

int tst_000()
{
    std::thread ths[10];
    for(int i = 0; i < 10; ++i)
        ths[i] = std::thread(do_print_id, i);

    std::cout << "10 threads are ready to race...\n";
    go();

    for(auto &th : ths)
        th.join();

    return 0;
}

int cargo = 0;
bool shipment_avaliable()
{
    return (cargo != 0);
}

// consume thread
void consume(int n)
{
    for(int i = 0; i < n; ++i) {
        std::unique_lock<std::mutex> lck(mtx);
        cv.wait(lck, shipment_avaliable);
        std::cout << cargo << "\n";
        cargo = 0;
    }
}


int tst_001()
{
    std::thread thread_consumer(consume, 10);
    for(int i = 0; i < 10; ++i) {
        while(shipment_avaliable())
            std::this_thread::yield();
        std::unique_lock<std::mutex> lck(mtx);
        cargo = i + 1;
        cv.notify_one();
    }

    thread_consumer.join();
    return 0;
}

int value;
void do_print_value()
{
    std::cin >> value;
    cv.notify_one();
}

int tst_002()
{
    std::cout << "Please enter an integer(I'll be waiting on dot.)\n";
    std::thread thread_print(do_print_value);

    std::unique_lock<std::mutex> lck(mtx);
    while(cv.wait_for(lck, std::chrono::seconds(1)) == std::cv_status::timeout) {
        std::cout << ".";
        std::cout.flush();
    }

    std::cout << "You entered value: " << value << std::endl;
    thread_print.join();

    return 0;
}

void consumer()
{
    std::unique_lock<std::mutex> lck(mtx);
    while(cargo == 0) cv.wait(lck);  // 当cv等待在一把锁上时，其他线程是可以获取该锁的。
    std::cout << cargo << "\n";
    cargo = 0;
}

void producer(int id)
{
    std::unique_lock<std::mutex> lck(mtx);
    cargo = id;
    cv.notify_one();
}

int tst_003()
{
    std::thread consumers[10], producers[10];

    for(int i = 0; i < 10; ++i) {
        consumers[i] = std::thread(consumer);
        producers[i] = std::thread(producer, i + 1);
    }
    cv.notify_all();

    for(int i = 0; i < 10; ++i) {
        consumers[i].join();
        producers[i].join();
    }

    return 0;
}

void go_r0()
{
    std::unique_lock<std::mutex> lck(mtx);
    std::notify_all_at_thread_exit(cv, std::move(lck));
    ready = true;
}

int tst_004()
{
    std::thread ths[10];
    for(int i = 0; i < 10; ++i)
        ths[i] = std::thread(do_print_id, i + 1);
    std::cout << "10 threads are ready to race...\n";

    std::thread(go_r0).detach();

    for(auto &th : ths)
        th.join();

    return 0;
}

int main()
{
    tst_003();
    return 0;
}
